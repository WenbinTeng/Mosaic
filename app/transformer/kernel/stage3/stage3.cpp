#include "stage3.hpp"

int8_t gelu_fused(int32_t gelu_in, float M_stage3, int b_int, int c_int, int shift_int) {
    const int constant = 14;

    int32_t sign = (gelu_in >= 0) ? 1 : -1;
    int32_t val_abs = gelu_in * sign;
    int32_t abs_int = std::min(val_abs, -1 * b_int);
    int32_t intermediate = (abs_int + b_int);
    int32_t y_int = sign * (intermediate * intermediate + c_int);
    int32_t sigmoid_int = y_int / (1 << constant);

    gelu_in = gelu_in * (sigmoid_int + shift_int);

    return int8_t(gelu_in * M_stage3);
}

void read_A(int8_t *A, hls::stream<int8_t> &A_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE3; ++it)
    for (int jt = 0; jt < CFG::ff_dim / TILE_SIZE3; ++jt)
    for (int kt = 0; kt < CFG::model_dim / TILE_SIZE3; ++kt)
        for (int k = 0; k < TILE_SIZE3; ++k)
            for (int i = 0; i < TILE_SIZE3; ++i) {
                A_stream.write(A[(kt * TILE_SIZE3 + k) * CFG::seq_len + it * TILE_SIZE3 + i]);
            }
}

void read_B(int8_t *B, hls::stream<int8_t> &B_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE3; ++it)
    for (int jt = 0; jt < CFG::ff_dim / TILE_SIZE3; ++jt)
    for (int kt = 0; kt < CFG::model_dim / TILE_SIZE3; ++kt)
        for (int k = 0; k < TILE_SIZE3; ++k)
            for (int j = 0; j < TILE_SIZE3; ++j) {
                B_stream.write(B[(kt * TILE_SIZE3 + k) * CFG::ff_dim + jt * TILE_SIZE3 + j]);
            }
}

void read_bias(int32_t *bias, hls::stream<int32_t> &bias_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE3; ++it)
    for (int jt = 0; jt < CFG::ff_dim / TILE_SIZE3; ++jt)
        for (int i = 0; i < TILE_SIZE3; ++i)
            for (int j = 0; j < TILE_SIZE3; ++j) {
                bias_stream.write(bias[jt * TILE_SIZE3 + j]);
            }
}

void write_out3(int8_t *out, hls::stream<int8_t> &out_stream) {
    for (int it = 0; it < CFG::seq_len / TILE_SIZE3; ++it)
    for (int jt = 0; jt < CFG::ff_dim / TILE_SIZE3; ++jt)
        for (int i = 0; i < TILE_SIZE3; ++i)
            for (int j = 0; j < TILE_SIZE3; ++j) {
                out[(jt * TILE_SIZE3 + j) * CFG::seq_len + it * TILE_SIZE3 + i] = out_stream.read();
            }
}

void linear_fused(hls::stream<int8_t> &A_stream,
                  hls::stream<int8_t> &B_stream,
                  hls::stream<int32_t> &bias_stream,
                  hls::stream<int8_t> &out_stream,
                  float M_gelu, float M_stage3) {
    // compute fused gelu constants
    const float k = 1.4142;
    const int constant = 14;
    const float coef_0 = -0.2888;
    const float coef_1 = -1.769;
    const float coef_2 = 1 / coef_0;

    // int_erf
    float int_erf_scaling = M_gelu / k;
    int b_int = int(coef_1 / int_erf_scaling);
    int c_int = int(coef_2 / (int_erf_scaling * int_erf_scaling));
    float sigmoid_scaling_factor = int_erf_scaling * int_erf_scaling * coef_0;
    sigmoid_scaling_factor = sigmoid_scaling_factor * (1 << constant);

    int32_t shift_int = int32_t(1 / sigmoid_scaling_factor);

    // buffers for tile mmult
    int32_t out_block[TILE_SIZE3][TILE_SIZE3];
    int8_t B_line[TILE_SIZE3];
    int8_t A_T_line[TILE_SIZE3];

#pragma HLS array_partition dim = 2 complete variable = out_block
#pragma HLS array_partition dim = 1 complete variable = B_line

    for (int it = 0; it < CFG::seq_len / TILE_SIZE3; ++it) {
        for (int jt = 0; jt < CFG::ff_dim / TILE_SIZE3; ++jt) {
            // initialize output with bias
            for (int i = 0; i < TILE_SIZE3; ++i) {
                for (int j = 0; j < TILE_SIZE3; ++j) {
                    out_block[i][j] = bias_stream.read();
                }
            }

            for (int kt = 0; kt < CFG::model_dim / TILE_SIZE3; ++kt) {
                for (int k = 0; k < TILE_SIZE3; ++k) {

                    // read B values into vector
                    for (int j = 0; j < TILE_SIZE3; ++j) {
                        B_line[j] = B_stream.read();
                    }
                    for (int i = 0; i < TILE_SIZE3; ++i) {
                        A_T_line[i] = A_stream.read();
                    }

                    for (int i = 0; i < TILE_SIZE3; ++i) {
#pragma HLS PIPELINE
                        int8_t Ai = A_T_line[i];
                        for (int j = 0; j < TILE_SIZE3; ++j) {
#pragma HLS unroll
                            out_block[i][j] += Ai * B_line[j];
                        }
                    }
                }
            }

            // apply gelu and write output
            for (int i = 0; i < TILE_SIZE3; ++i) {
                for (int j = 0; j < TILE_SIZE3; ++j) {
                    out_stream.write(gelu_fused(out_block[i][j], M_stage3, b_int, c_int, shift_int));
                }
            }
        }
    }
}

void stage3(int8_t *fc_in,
            int8_t *dense_weight,
            int32_t *dense_bias,
            int8_t *dense_out,
            float M_gelu, float M_stage3) {
    static hls::stream<int8_t> A_stream("A_stream");
    static hls::stream<int8_t> B_stream("B_stream");
    static hls::stream<int32_t> bias_stream("bias_stream");
    static hls::stream<int8_t> out_stream("out_stream");

#pragma HLS stream variable = A_stream depth = 8
#pragma HLS stream variable = B_stream depth = 8
#pragma HLS stream variable = bias_stream depth = 8
#pragma HLS stream variable = out_stream depth = 8

#pragma HLS dataflow

    read_A(fc_in, A_stream);
    read_B(dense_weight, B_stream);
    read_bias(dense_bias, bias_stream);
    linear_fused(A_stream, B_stream, bias_stream, out_stream, M_gelu, M_stage3);
    write_out3(dense_out, out_stream);
}
