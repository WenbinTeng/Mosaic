#include "stage4.hpp"

int16_t requant(int32_t ob, int8_t sb, float Md, float Mr) {
    int8_t out8 = int8_t(ob * Md) + sb;
    return int16_t(out8 * Mr);
}

void read_A4(int8_t *A, hls::stream<int8_t> &A_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE4; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE4; ++jt)
    for (int kt = 0; kt < CFG::ff_dim / TILE_SIZE4; ++kt)
        for (int k = 0; k < TILE_SIZE4; ++k)
            for (int i = 0; i < TILE_SIZE4; ++i) {
                A_stream.write(A[(kt * TILE_SIZE4 + k) * CFG::seq_len + it * TILE_SIZE4 + i]);
            }
}

void read_B4(int8_t *B, hls::stream<int8_t> &B_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE4; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE4; ++jt)
    for (int kt = 0; kt < CFG::ff_dim / TILE_SIZE4; ++kt)
        for (int k = 0; k < TILE_SIZE4; ++k)
            for (int j = 0; j < TILE_SIZE4; ++j) {
                B_stream.write(B[(kt * TILE_SIZE4 + k) * CFG::model_dim + jt * TILE_SIZE4 + j]);
            }
}

void read_bias4(int32_t *bias, hls::stream<int32_t> &bias_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE4; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE4; ++jt)
        for (int i = 0; i < TILE_SIZE4; ++i)
            for (int j = 0; j < TILE_SIZE4; ++j) {
                bias_stream.write(bias[jt * TILE_SIZE4 + j]);
            }
}

void read_skip4(int8_t *skip, hls::stream<int8_t> &skip_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE4; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE4; ++jt)
        for (int i = 0; i < TILE_SIZE4; ++i)
            for (int j = 0; j < TILE_SIZE4; ++j) {
                skip_stream.write(skip[(jt * TILE_SIZE4 + j) * CFG::seq_len + it * TILE_SIZE4 + i]);
            }
}

void write_out4(int it, int jt,
                int32_t out_block[TILE_SIZE4][TILE_SIZE4],
                int8_t skip_buff[TILE_SIZE4][TILE_SIZE4],
                float Md, float Mr,
                int16_t *out) {
    for (int i = 0; i < TILE_SIZE4; ++i) {
        for (int j = 0; j < TILE_SIZE4; ++j) {
            out[(it * TILE_SIZE4 + i) * CFG::model_dim + jt * TILE_SIZE4 + j] = requant(out_block[i][j], skip_buff[i][j], Md, Mr);
        }
    }
}

void linear_fused4(hls::stream<int8_t> &A_stream,
                   hls::stream<int8_t> &B_stream,
                   hls::stream<int32_t> &bias_stream,
                   hls::stream<int8_t> &skip_stream,
                   float M_dense, float M_residual,
                   int16_t *out) {

    int32_t out_block[TILE_SIZE4][TILE_SIZE4];
    int8_t skip_buff[TILE_SIZE4][TILE_SIZE4];
    int8_t B_line[TILE_SIZE4];
    int8_t A_T_line[TILE_SIZE4];

#pragma HLS array_partition dim = 2 complete variable = out_block
#pragma HLS array_partition dim = 1 complete variable = B_line

    for (int it = 0; it < CFG::seq_len / TILE_SIZE4; ++it) {
        for (int jt = 0; jt < CFG::model_dim / TILE_SIZE4; ++jt) {
            // initialize output with bias
            for (int i = 0; i < TILE_SIZE4; ++i) {
                for (int j = 0; j < TILE_SIZE4; ++j) {
                    out_block[i][j] = bias_stream.read();
                    skip_buff[i][j] = skip_stream.read();
                }
            }

            for (int kt = 0; kt < CFG::ff_dim / TILE_SIZE4; ++kt) {
                for (int k = 0; k < TILE_SIZE4; ++k) {
                    // read B values into vector
                    for (int j = 0; j < TILE_SIZE4; ++j) {
                        B_line[j] = B_stream.read();
                    }
                    for (int i = 0; i < TILE_SIZE4; ++i) {
                        A_T_line[i] = A_stream.read();
                    }

                    for (int i = 0; i < TILE_SIZE4; ++i) {
// #pragma HLS unroll factor=4
#pragma HLS PIPELINE
                        int8_t Ai = A_T_line[i];
                        for (int j = 0; j < TILE_SIZE4; ++j) {
#pragma HLS unroll
                            out_block[i][j] += Ai * B_line[j];
                        }
                    }
                }
            }

            write_out4(it, jt, out_block, skip_buff, M_dense, M_residual, out);
        }
    }
}

void linear_dataflow4(int8_t *fc_in,
                      int8_t *dense_weight,
                      int32_t *dense_bias,
                      int8_t *skip,
                      float M_dense_acc, float M_residual,
                      int16_t *out_buff) {
    static hls::stream<int8_t> A_stream("A_stream");
    static hls::stream<int8_t> B_stream("B_stream");
    static hls::stream<int32_t> bias_stream("bias_stream");
    static hls::stream<int8_t> skip_stream("skip_stream");
#pragma HLS stream variable = A_stream depth = 128
#pragma HLS stream variable = B_stream depth = 128
#pragma HLS stream variable = bias_stream depth = 128
#pragma HLS stream variable = skip_stream depth = 128

#pragma HLS dataflow

    read_A4(fc_in, A_stream);
    read_B4(dense_weight, B_stream);
    read_bias4(dense_bias, bias_stream);
    read_skip4(skip, skip_stream);
    linear_fused4(A_stream, B_stream, bias_stream, skip_stream, M_dense_acc, M_residual, out_buff);
}

void layernorm_fused4(int16_t *in,
                      int16_t *norm_weight,
                      int16_t *norm_bias,
                      float M_residual, float M_stage,
                      int8_t *out) {
    const int16_t C = int16_t(CFG::eps / M_residual);

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
        int16_t stdev = (int16_t)(sqrt(float(acc16 + C)));

        for (int j = 0; j < CFG::model_dim; ++j) {
            in[i * CFG::model_dim + j] /= stdev;
            int16_t acc16 = (int16_t)((in[i * CFG::model_dim + j] * norm_weight[j] + norm_bias[j]) * M_residual);
            out[i * CFG::model_dim + j] = (int8_t)(acc16 * M_stage);
        }
    }
}

void stage4(int8_t *fc_in,
            int8_t *skip,
            int8_t *dense_weight, int32_t *dense_bias, int8_t *dense_out,
            int16_t *norm_weight, int16_t *norm_bias,
            float M_dense_acc, float M_residual,float M_stage4) {
    int16_t fc_ln_buff[CFG::seq_len][CFG::model_dim];
    int16_t *buff = &fc_ln_buff[0][0];

    linear_dataflow4(fc_in, dense_weight, dense_bias, skip, M_dense_acc, M_residual, buff);
    layernorm_fused4(buff, norm_weight, norm_bias, M_residual, M_stage4, dense_out);
}