#include "stage2.hpp"

int16_t requant(int32_t ob, int8_t sb, float Md, float Mr) {
    int8_t out8 = int8_t(ob * Md) + sb;
    return int16_t(out8 * Mr);
}

void softmax_fused(int32_t* input, int8_t* output, int start_idx, float M_softmax) {
	int i;
	double max, sum, constant;

    max = input[0];
    for (i = 0; i < CFG::seq_len; ++i) {
        if (max < input[i]) {
            max = input[i];
        }
    }

    sum = 0.0;
	for (i = 0; i < CFG::seq_len; ++i) {
		sum += exp(input[i] - max);
	}

    constant = max + log(sum);
    for (i = 0; i < CFG::seq_len; ++i) {
        output[start_idx + i] = (int8_t)(exp(input[i] - constant) * M_softmax);
    }
}

void attention_scores_fused(int8_t *query, int8_t *key, int8_t *attn_score_out, float M_attn_probs) {

    int32_t divisor = std::sqrt(CFG::model_dim);
    int32_t score_row[CFG::seq_len];
    int32_t query_row[CFG::head_num];
    int32_t key_row[CFG::head_num];

#pragma HLS array_partition variable = query_row type = complete
#pragma HLS array_partition variable = key_row type = complete

    for (int n = 0; n < CFG::head_num; ++n) {
        // compute matmul head_num times
        for (int i = 0; i < CFG::seq_len; ++i) {
            for (int k = 0; k < CFG::head_num; ++k) {
                query_row[k] = query[i * CFG::head_num * CFG::head_num + n * CFG::head_num + k];
            }
            for (int j = 0; j < CFG::seq_len; ++j) {
                for (int k = 0; k < CFG::head_num; ++k) {
                    key_row[k] = key[j * CFG::head_num * CFG::head_num + n * CFG::head_num + k];
                }

                int32_t acc = 0;
                for (int k = 0; k < CFG::head_num; ++k) { // acc += query[n,i,k] * key[n,k,j]
#pragma HLS unroll
                    acc += query_row[k] * key_row[k];
                }
                // out[n,i,j] = acc
                score_row[j] = acc / divisor;
            }
            softmax_fused(score_row, attn_score_out, n * CFG::seq_len * CFG::seq_len + i * CFG::seq_len, M_attn_probs);
        }
    }
}

void attention_values_fused(int8_t* score, int8_t *value, int8_t* attn_value_out, float M_attn_out) {

    int32_t row_buf[CFG::head_num];
    int8_t score_row[CFG::seq_len];
    int8_t value_row[CFG::head_num];
#pragma HLS array_partition variable = row_buf type = complete
#pragma HLS array_partition variable = value_row type = complete

    for (int n = 0; n < CFG::head_num; ++n) {
        for (int i = 0; i < CFG::seq_len; ++i) {
            for (int k = 0; k < CFG::seq_len; ++k) {
                score_row[k] = score[n * CFG::seq_len * CFG::seq_len + i * CFG::seq_len + k];
            }
            for (int j = 0; j < CFG::head_num; ++j) {
                row_buf[j] = 0;
            }
            for (int k = 0; k < CFG::seq_len; ++k) {
                for (int j = 0; j < CFG::head_num; ++j) {
                    value_row[j] = value[k * CFG::head_num * CFG::head_num + n * CFG::head_num + j];
                }

                int8_t score_k = score_row[k];
                for (int j = 0; j < CFG::head_num; ++j) {
#pragma HLS unroll
                    // attn_out[n][i][j] += score[n][i][k] * value[n][k][j]
                    row_buf[j] += score_k * value_row[j];
                }
            }
            for (int j = 0; j < CFG::head_num; ++j) {
                attn_value_out[i * CFG::head_num * CFG::head_num + n * CFG::head_num + j] = (int8_t)(row_buf[j] * M_attn_out);
            }
        }
    }
}

void read_weight2(int8_t *weight, hls::stream<int8_t> &weight_stream) {

    for (int it = 0; it < CFG::seq_len / TILE_SIZE2; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE2; ++jt)
    for (int kt = 0; kt < CFG::model_dim / TILE_SIZE2; ++kt)
        for (int k = 0; k < TILE_SIZE2; ++k)
            for (int j = 0; j < TILE_SIZE2; ++j) {
                weight_stream.write(weight[(kt * TILE_SIZE2 + k) * CFG::model_dim + jt * TILE_SIZE2 + j]);
            }
}

void read_bias2(int32_t *bias, hls::stream<int32_t> &bias_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE2; ++it) 
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE2; ++jt) 
        for (int i = 0; i < TILE_SIZE2; ++i) 
            for (int j = 0; j < TILE_SIZE2; ++j) {
                bias_stream.write(bias[jt * TILE_SIZE2 + j]);
            }

}

void read_skip2(int8_t *skip, hls::stream<int8_t> &skip_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE2; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE2; ++jt)
        for (int i = 0; i < TILE_SIZE2; ++i)
            for (int j = 0; j < TILE_SIZE2; ++j) {
                skip_stream.write(skip[(jt * TILE_SIZE2 + j) * CFG::seq_len + it * TILE_SIZE2 + i]);
            }
}

void write_output2(int it, int jt,
                   int32_t out_block[TILE_SIZE2][TILE_SIZE2],
                   int8_t skip_buff[TILE_SIZE2][TILE_SIZE2],
                   float Md, float Mr,
                   int16_t *out) {
    for (int i = 0; i < TILE_SIZE2; ++i) {
        for (int j = 0; j < TILE_SIZE2; ++j) {
            out[(it * TILE_SIZE2 + i) * CFG::model_dim + jt * TILE_SIZE2 + j] = requant(out_block[i][j], skip_buff[i][j], Md, Mr);
        }
    }
}

void linear_fused2(int8_t *A, hls::stream<int8_t> &weight_stream,
                   hls::stream<int32_t> &bias_stream,
                   hls::stream<int8_t> &skip_stream,
                   float M_dense, float M_residual,
                   int16_t *out) {

    // buffers for tile mmult
    int32_t out_block[TILE_SIZE2][TILE_SIZE2];
    int8_t skip_buff[TILE_SIZE2][TILE_SIZE2];
    int8_t weight_line[TILE_SIZE2];

#pragma HLS array_partition dim = 2 complete variable = out_block
#pragma HLS array_partition dim = 1 complete variable = weight_line

    for (int it = 0; it < CFG::seq_len / TILE_SIZE2; ++it) {
        for (int jt = 0; jt < CFG::model_dim / TILE_SIZE2; ++jt) {
            // initialize output with bias
            for (int i = 0; i < TILE_SIZE2; ++i) {
                for (int j = 0; j < TILE_SIZE2; ++j) {
#pragma HLS pipeline II = 1
                    out_block[i][j] = bias_stream.read();
                    skip_buff[i][j] = skip_stream.read();
                }
            }

            for (int kt = 0; kt < CFG::model_dim / TILE_SIZE2; ++kt) {
                for (int k = 0; k < TILE_SIZE2; ++k) {

                    // read B values into vector
                    for (int j = 0; j < TILE_SIZE2; ++j) {
                        weight_line[j] = weight_stream.read();
                    }

                    for (int i = 0; i < TILE_SIZE2; ++i) {
#pragma HLS PIPELINE II = 1
                        int8_t Ai = A[(it * TILE_SIZE2 + i) * CFG::model_dim + kt * TILE_SIZE2 + k];
                        for (int j = 0; j < TILE_SIZE2; ++j) {
#pragma HLS unroll
                            out_block[i][j] += Ai * weight_line[j];
                        }
                    }
                }
            }

            write_output2(it, jt, out_block, skip_buff, M_dense, M_residual, out);
        }
    }
}

void linear_dataflow2(int8_t *in_buff,
                      int8_t *weight,
                      int32_t *bias,
                      int8_t *skip,
                      float M_dense_out, float M_residual,
                      int16_t *out_buff) {
#pragma HLS dataflow
    static hls::stream<int8_t> dense_weight_stream("dense_weight_stream");
    static hls::stream<int32_t> dense_bias_stream("dense_bias_stream");
    static hls::stream<int8_t> skip_stream("skip_stream");

#pragma HLS stream variable = dense_weight_stream depth = 128
#pragma HLS stream variable = dense_bias_stream depth = 128
#pragma HLS stream variable = skip_stream depth = 128

    read_weight2(weight, dense_weight_stream);
    read_bias2(bias, dense_bias_stream);
    read_skip2(skip, skip_stream);

    linear_fused2(in_buff, dense_weight_stream, dense_bias_stream, skip_stream, M_dense_out, M_residual, out_buff);
}

void layernorm_fused2(int16_t *in,
                      int16_t *norm_weight,
                      int16_t *norm_bias,
                      float M_residual, float M_stage,
                      int8_t *out) {
    const int16_t C = (int16_t)(CFG::eps / M_residual);

    for (int i = 0; i < CFG::seq_len; ++i) {
        int32_t macc = 0;
        for (int j = 0; j < CFG::model_dim; ++j) {
            macc += in[i * CFG::model_dim + j];
        }
        int16_t m = (int16_t)(macc / CFG::model_dim);
        for (int j = 0; j < CFG::model_dim; ++j) {
            in[i * CFG::model_dim + j] = in[i * CFG::model_dim + j] - m;
        }
    }

    for (int i = 0; i < CFG::seq_len; ++i) {
        int16_t acc16 = 0;
        for (int j = 0; j < CFG::model_dim; ++j) {
            acc16 += (int16_t)(in[i * CFG::model_dim + j] * (int32_t)(in[i * CFG::model_dim + j]) / CFG::model_dim);
        }
        int16_t stdev = int16_t(sqrt(float(acc16 + C)));
        for (int j = 0; j < CFG::model_dim; ++j) {
            in[i * CFG::model_dim + j] /= stdev;
            int16_t acc16 = (int16_t)((in[i * CFG::model_dim + j] * norm_weight[j] + norm_bias[j]) * M_residual);
            out[j * CFG::seq_len + i] = (int8_t)(acc16 * M_stage);
        }
    }
}

void stage2(int8_t *query_in, int8_t *key_in, int8_t *value_in, int8_t *skip_in,
            int8_t *dense_weight_t, int32_t *dense_bias,
            float M_attn_probs, float M_attn_out, float M_dense_out, float M_residual,
            int16_t *norm_weight, int16_t *norm_bias, float M_stage2,
            int8_t *stage2_out) {
    int8_t attn_score_buff[CFG::head_num * CFG::seq_len * CFG::seq_len];
    int8_t attn_value_buff[CFG::seq_len * CFG::model_dim];
    int16_t lin_buff[CFG::seq_len * CFG::model_dim];

    attention_scores_fused(query_in, key_in, attn_score_buff, M_attn_probs);
    attention_values_fused(attn_score_buff, value_in, attn_value_buff, M_attn_out);
    linear_dataflow2(attn_value_buff, dense_weight_t, dense_bias, skip_in, M_dense_out, M_residual, lin_buff);
    layernorm_fused2(lin_buff, norm_weight, norm_bias, M_residual, M_stage2, stage2_out);
}
