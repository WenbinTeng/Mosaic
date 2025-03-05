#include "top.hpp"

void read_input_data(float input[32][32], hls::stream<float>& in_stream) {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            in_stream.write(input[i][j]);
        }
    }
}

void read_conv1_data(float weight[6][5][5], float bias[6],
                     hls::stream<float>& weight_stream,
                     hls::stream<float>& bias_stream) {
    for (int c = 0; c < 6; c++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                weight_stream.write(weight[c][i][j]);
            }
        }
        bias_stream.write(bias[c]);
    }
}

void read_conv3_data(float weight[16][6][5][5], float bias[16],
                     hls::stream<float>& weight_stream,
                     hls::stream<float>& bias_stream) {
    for (int co = 0; co < 16; co++) {
        for (int ci = 0; ci < 6; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_conv5_data(float weight[120][16][5][5], float bias[120],
                     hls::stream<float>& weight_stream,
                     hls::stream<float>& bias_stream) {
    for (int co = 0; co < 120; co++) {
        for (int ci = 0; ci < 16; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_full6_data(float weight[84][120], float bias[84],
                     hls::stream<float>& weight_stream,
                     hls::stream<float>& bias_stream) {
    for (int c = 0; c < 84; c++) {
        for (int n = 0; n < 120; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void read_out_data(float weight[10][84], float bias[10],
                   hls::stream<float>& weight_stream,
                   hls::stream<float>& bias_stream) {
    for (int c = 0; c < 10; c++) {
        for (int n = 0; n < 84; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void write_output_data(float output[10], hls::stream<float>& out_stream) {
    for (int n = 0; n < 10; n++) {
        output[n] = out_stream.read();
    }
}

void top(
    float input[32][32],
    float conv1_weight[6][5][5],
    float conv1_bias[6],
    float conv3_weight[16][6][5][5],
    float conv3_bias[16],
    float conv5_weight[120][16][5][5],
    float conv5_bias[120],
    float full6_weight[84][120],
    float full6_bias[84],
    float out_weight[10][84],
    float out_bias[10],
    float output[10]
) {

    hls_thread_local hls::stream<float> in_stream("in_stream");
    hls_thread_local hls::stream<float> conv1_weight_stream("conv1_weight_stream");
    hls_thread_local hls::stream<float> conv1_bias_stream("conv1_bias_stream");
    hls_thread_local hls::stream<float> conv3_weight_stream("conv3_weight_stream");
    hls_thread_local hls::stream<float> conv3_bias_stream("conv3_bias_stream");
    hls_thread_local hls::stream<float> conv5_weight_stream("conv5_weight_stream");
    hls_thread_local hls::stream<float> conv5_bias_stream("conv5_bias_stream");
    hls_thread_local hls::stream<float> full6_weight_stream("full6_weight_stream");
    hls_thread_local hls::stream<float> full6_bias_stream("full6_bias_stream");
    hls_thread_local hls::stream<float> out_weight_stream("out_weight_stream");
    hls_thread_local hls::stream<float> out_bias_stream("out_bias_stream");
    hls_thread_local hls::stream<float> out_stream("out_stream");

    hls_thread_local hls::stream<float> conv1_output_stream("conv1_output_stream");
    hls_thread_local hls::stream<float> samp2_output_stream("samp2_output_stream");
    hls_thread_local hls::stream<float> conv3_output_stream("conv3_output_stream");
    hls_thread_local hls::stream<float> samp4_output_stream("samp4_output_stream");
    hls_thread_local hls::stream<float> conv5_output_stream("conv5_output_stream");
    hls_thread_local hls::stream<float> full6_output_stream("full6_output_stream");

    hls_thread_local hls::task t1(read_input_data, input, in_stream);

    hls_thread_local hls::task t2(read_conv1_data, conv1_weight, conv1_bias, conv1_weight_stream, conv1_bias_stream);
    hls_thread_local hls::task t3(conv1, in_stream, conv1_weight_stream, conv1_bias_stream, conv1_output_stream);
    hls_thread_local hls::task t4(samp2, conv1_output_stream, samp2_output_stream);

    hls_thread_local hls::task t5(read_conv3_data, conv3_weight, conv3_bias, conv3_weight_stream, conv3_bias_stream);
    hls_thread_local hls::task t6(conv3, samp2_output_stream, conv3_weight_stream, conv3_bias_stream, conv3_output_stream);
    hls_thread_local hls::task t7(samp4, conv3_output_stream, samp4_output_stream);

    hls_thread_local hls::task t8(read_conv5_data, conv5_weight, conv5_bias, conv5_weight_stream, conv5_bias_stream);
    hls_thread_local hls::task t9(conv5, samp4_output_stream, conv5_weight_stream, conv5_bias_stream, conv5_output_stream);

    hls_thread_local hls::task t10(read_full6_data, full6_weight, full6_bias, full6_weight_stream, full6_bias_stream);
    hls_thread_local hls::task t11(full6, conv5_output_stream, full6_weight_stream, full6_bias_stream, full6_output_stream);

    hls_thread_local hls::task t12(read_out_data, out_weight, out_bias, out_weight_stream, out_bias_stream);
    hls_thread_local hls::task t13(out, full6_output_stream, out_weight_stream, out_bias_stream, out_stream);

    hls_thread_local hls::task t14(write_output_data, output, out_stream);
}
