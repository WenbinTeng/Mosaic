#include "top.hpp"

void read_input_data(DTYPE input[32][32], hls::stream<DTYPE>& in_stream) {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            in_stream.write(input[i][j]);
        }
    }
}

void read_conv1_data(DTYPE weight[6][5][5], DTYPE bias[6],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 6; c++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                weight_stream.write(weight[c][i][j]);
            }
        }
        bias_stream.write(bias[c]);
    }
}

void read_conv2_data(DTYPE weight[16][6][5][5], DTYPE bias[16],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
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

void read_conv3_data(DTYPE weight[120][16][5][5], DTYPE bias[120],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
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

void read_full4_data(DTYPE weight[84][120], DTYPE bias[84],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 84; c++) {
        for (int n = 0; n < 120; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void read_full5_data(DTYPE weight[10][84], DTYPE bias[10],
                   hls::stream<DTYPE>& weight_stream,
                   hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 10; c++) {
        for (int n = 0; n < 84; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void write_output_data(DTYPE output[10], hls::stream<DTYPE>& out_stream) {
    for (int n = 0; n < 10; n++) {
        output[n] = out_stream.read();
    }
}

void top(
    DTYPE input[32][32],
    DTYPE conv1_weight[6][5][5],
    DTYPE conv1_bias[6],
    DTYPE conv2_weight[16][6][5][5],
    DTYPE conv2_bias[16],
    DTYPE conv3_weight[120][16][5][5],
    DTYPE conv3_bias[120],
    DTYPE full4_weight[84][120],
    DTYPE full4_bias[84],
    DTYPE full5_weight[10][84],
    DTYPE full5_bias[10],
    DTYPE output[10]
) {

    hls_thread_local hls::stream<DTYPE> in_stream("in_stream");
    hls_thread_local hls::stream<DTYPE> conv1_weight_stream("conv1_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv1_bias_stream("conv1_bias_stream");
    hls_thread_local hls::stream<DTYPE> conv2_weight_stream("conv2_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv2_bias_stream("conv2_bias_stream");
    hls_thread_local hls::stream<DTYPE> conv3_weight_stream("conv3_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv3_bias_stream("conv3_bias_stream");
    hls_thread_local hls::stream<DTYPE> full4_weight_stream("full4_weight_stream");
    hls_thread_local hls::stream<DTYPE> full4_bias_stream("full4_bias_stream");
    hls_thread_local hls::stream<DTYPE> full5_weight_stream("full5_weight_stream");
    hls_thread_local hls::stream<DTYPE> full5_bias_stream("full5_bias_stream");
    hls_thread_local hls::stream<DTYPE> out_stream("out_stream");

    hls_thread_local hls::stream<DTYPE> conv1_out_stream("conv1_out_stream");
    hls_thread_local hls::stream<DTYPE> maxp1_out_stream("maxp1_out_stream");
    hls_thread_local hls::stream<DTYPE> conv2_out_stream("conv2_out_stream");
    hls_thread_local hls::stream<DTYPE> maxp2_out_stream("maxp2_out_stream");
    hls_thread_local hls::stream<DTYPE> conv3_out_stream("conv3_out_stream");
    hls_thread_local hls::stream<DTYPE> full4_out_stream("full4_out_stream");

    hls_thread_local hls::task t1(read_input_data, input, in_stream);
    
    hls_thread_local hls::task t2(read_conv1_data, conv1_weight, conv1_bias, conv1_weight_stream, conv1_bias_stream);
    hls_thread_local hls::task t3(conv1, in_stream, conv1_weight_stream, conv1_bias_stream, conv1_out_stream);
    hls_thread_local hls::task t4(maxp1, conv1_out_stream, maxp1_out_stream);

    hls_thread_local hls::task t5(read_conv2_data, conv2_weight, conv2_bias, conv2_weight_stream, conv2_bias_stream);
    hls_thread_local hls::task t6(conv2, maxp1_out_stream, conv2_weight_stream, conv2_bias_stream, conv2_out_stream);
    hls_thread_local hls::task t7(maxp2, conv2_out_stream, maxp2_out_stream);

    hls_thread_local hls::task t8(read_conv3_data, conv3_weight, conv3_bias, conv3_weight_stream, conv3_bias_stream);
    hls_thread_local hls::task t9(conv3, maxp2_out_stream, conv3_weight_stream, conv3_bias_stream, conv3_out_stream);

    hls_thread_local hls::task t10(read_full4_data, full4_weight, full4_bias, full4_weight_stream, full4_bias_stream);
    hls_thread_local hls::task t11(full4, conv3_out_stream, full4_weight_stream, full4_bias_stream, full4_out_stream);

    hls_thread_local hls::task t12(read_full5_data, full5_weight, full5_bias, full5_weight_stream, full5_bias_stream);
    hls_thread_local hls::task t13(full5, full4_out_stream, full5_weight_stream, full5_bias_stream, out_stream);

    hls_thread_local hls::task t14(write_output_data, output, out_stream);
}
