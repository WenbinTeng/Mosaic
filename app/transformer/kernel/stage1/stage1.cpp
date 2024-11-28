#include "stage1.hpp"

void read_input1(int8_t *in,
                 hls::stream<int8_t> &in_1,
                 hls::stream<int8_t> &in_2,
                 hls::stream<int8_t> &in_3) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE1; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE1; ++jt)
    for (int kt = 0; kt < CFG::model_dim / TILE_SIZE1; ++kt)
        for (int k = 0; k < TILE_SIZE1; ++k)
            for (int i = 0; i < TILE_SIZE1; ++i) {
                int8_t A_val = in[(kt * TILE_SIZE1 + k) * CFG::seq_len + it * TILE_SIZE1 + i];
                in_1.write(A_val);
                in_2.write(A_val);
                in_3.write(A_val);
            }
}

void read_weights1(int8_t *weights, hls::stream<int8_t> &weights_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE1; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE1; ++jt)
    for (int kt = 0; kt < CFG::model_dim / TILE_SIZE1; ++kt)
        for (int k = 0; k < TILE_SIZE1; ++k)
            for (int j = 0; j < TILE_SIZE1; ++j) {
                weights_stream.write(weights[(kt * TILE_SIZE1 + k) * CFG::model_dim + jt * TILE_SIZE1 + j]);
            }
}

void read_bias1(int32_t *bias, hls::stream<int32_t> &bias_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE1; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE1; ++jt)
        for (int i = 0; i < TILE_SIZE1; ++i)
            for (int j = 0; j < TILE_SIZE1; ++j) {
                bias_stream.write(bias[jt * TILE_SIZE1 + j]);
            }
}

void write_out1(int8_t *out, hls::stream<int8_t> &out_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE1; ++it)
    for (int jt = 0; jt < CFG::model_dim / TILE_SIZE1; ++jt)
        for (int i = 0; i < TILE_SIZE1; ++i)
            for (int j = 0; j < TILE_SIZE1; ++j) {
                out[(it * TILE_SIZE1 + i) * CFG::model_dim + jt * TILE_SIZE1 + j] = out_stream.read();
            }
}

/*
    A: NxK
    B: KxM
    out: NxM
    Bias: 1xM
*/
void linear_fused1(hls::stream<int8_t> &A_stream,
                   hls::stream<int8_t> &B_stream,
                   hls::stream<int32_t> &bias_stream,
                   hls::stream<int8_t> &out_stream,
                   const float M_scale) {
    // buffers for tile mmult
    int32_t out_block[TILE_SIZE1][TILE_SIZE1];
    int8_t B_line[TILE_SIZE1];
    int8_t A_line[TILE_SIZE1];

#pragma HLS array_partition dim = 2 complete variable = out_block
#pragma HLS array_partition dim = 1 complete variable = B_line

    for (int it = 0; it < CFG::seq_len / TILE_SIZE1; ++it) {
        for (int jt = 0; jt < CFG::model_dim / TILE_SIZE1; ++jt) {
            // initialize output with bias
            for (int i = 0; i < TILE_SIZE1; ++i) {
                for (int j = 0; j < TILE_SIZE1; ++j) {
                    out_block[i][j] = bias_stream.read();
                }
            }

            for (int kt = 0; kt < CFG::model_dim / TILE_SIZE1; ++kt) {
                for (int k = 0; k < TILE_SIZE1; ++k) {

                    // read B values into vector
                    for (int j = 0; j < TILE_SIZE1; ++j) {
                        B_line[j] = B_stream.read();
                    }
                    for (int i = 0; i < TILE_SIZE1; ++i) {
                        A_line[i] = A_stream.read();
                    }

                    for (int i = 0; i < TILE_SIZE1; ++i) {
#pragma HLS PIPELINE II = 1
                        int8_t Ai = A_line[i];
                        for (int j = 0; j < TILE_SIZE1; ++j) {
#pragma HLS unroll
                            out_block[i][j] += Ai * B_line[j];
                        }
                    }
                }
            }

            for (int i = 0; i < TILE_SIZE1; ++i) {
                for (int j = 0; j < TILE_SIZE1; ++j) {
                    out_stream.write(int8_t(out_block[i][j] * M_scale));
                }
            }
        }
    }
}

void stage1(int8_t *in,
            int8_t *query_out, int8_t *key_out, int8_t *value_out,
            int8_t *query_weight, int32_t *query_bias,
            int8_t *key_weight, int32_t *key_bias,
            int8_t *value_weight, int32_t *value_bias,
            float M_query, float M_key, float M_value) {
#pragma HLS dataflow

    static hls::stream<int8_t> in_query("in_query_stream");
    static hls::stream<int8_t> in_key("in_key_stream");
    static hls::stream<int8_t> in_value("in_value_stream");
#pragma HLS stream variable = in_query depth = 128
#pragma HLS stream variable = in_key depth = 128
#pragma HLS stream variable = in_value depth = 128

    static hls::stream<int8_t> query_weight_stream("query_weight_stream");
    static hls::stream<int8_t> key_weight_stream("key_weight_stream");
    static hls::stream<int8_t> value_weight_stream("value_weight_stream");
#pragma HLS stream variable = query_weight_stream depth = 128
#pragma HLS stream variable = key_weight_stream depth = 128
#pragma HLS stream variable = value_weight_stream depth = 128

    static hls::stream<int32_t> query_bias_stream("query_bias_stream");
    static hls::stream<int32_t> key_bias_stream("key_bias_stream");
    static hls::stream<int32_t> value_bias_stream("value_bias_stream");
#pragma HLS stream variable = query_bias_stream depth = 128
#pragma HLS stream variable = key_bias_stream depth = 128
#pragma HLS stream variable = value_bias_stream depth = 128

    static hls::stream<int8_t> query_out_stream("query_out_stream");
    static hls::stream<int8_t> key_out_stream("key_out_stream");
    static hls::stream<int8_t> value_out_stream("value_out_stream");

    read_input1(in, in_query, in_key, in_value);

    read_weights1(query_weight, query_weight_stream);
    read_weights1(key_weight, key_weight_stream);
    read_weights1(value_weight, value_weight_stream);

    read_bias1(query_bias, query_bias_stream);
    read_bias1(key_bias, key_bias_stream);
    read_bias1(value_bias, value_bias_stream);

    linear_fused1(in_query, query_weight_stream, query_bias_stream, query_out_stream, M_query);
    linear_fused1(in_key, key_weight_stream, key_bias_stream, key_out_stream, M_key);
    linear_fused1(in_value, value_weight_stream, value_bias_stream, value_out_stream, M_value);

    write_out1(query_out, query_out_stream);
    write_out1(key_out, key_out_stream);
    write_out1(value_out, value_out_stream);
}