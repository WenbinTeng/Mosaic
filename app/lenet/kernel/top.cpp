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

void read_conv3_data(DTYPE weight[16][6][5][5], DTYPE bias[16],
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

void read_conv5_data(DTYPE weight[120][16][5][5], DTYPE bias[120],
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

void read_full6_data(DTYPE weight[84][120], DTYPE bias[84],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 84; c++) {
        for (int n = 0; n < 120; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void read_out_data(DTYPE weight[10][84], DTYPE bias[10],
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
    DTYPE conv3_weight[16][6][5][5],
    DTYPE conv3_bias[16],
    DTYPE conv5_weight[120][16][5][5],
    DTYPE conv5_bias[120],
    DTYPE full6_weight[84][120],
    DTYPE full6_bias[84],
    DTYPE out_weight[10][84],
    DTYPE out_bias[10],
    DTYPE output[10]
) {

    hls_thread_local hls::stream<DTYPE> in_stream("in_stream");
    hls_thread_local hls::stream<DTYPE> layer1_conv_weight_stream("layer1_conv_weight_stream");
    hls_thread_local hls::stream<DTYPE> layer1_conv_bias_stream("layer1_conv_bias_stream");
    hls_thread_local hls::stream<DTYPE> layer3_conv_weight_stream("layer3_conv_weight_stream");
    hls_thread_local hls::stream<DTYPE> layer3_conv_bias_stream("layer3_conv_bias_stream");
    hls_thread_local hls::stream<DTYPE> layer5_conv_weight_stream("layer5_conv_weight_stream");
    hls_thread_local hls::stream<DTYPE> layer5_conv_bias_stream("layer5_conv_bias_stream");
    hls_thread_local hls::stream<DTYPE> layer6_fc_weight_stream("layer6_fc_weight_stream");
    hls_thread_local hls::stream<DTYPE> layer6_fc_bias_stream("layer6_fc_bias_stream");
    hls_thread_local hls::stream<DTYPE> layer7_out_weight_stream("layer7_out_weight_stream");
    hls_thread_local hls::stream<DTYPE> layer7_out_bias_stream("layer7_out_bias_stream");
    hls_thread_local hls::stream<DTYPE> out_stream("out_stream");

    hls_thread_local hls::stream<DTYPE> layer1_conv_output_stream("layer1_conv_output_stream");
    hls_thread_local hls::stream<DTYPE> layer2_maxp_output_stream("layer2_maxp_output_stream");
    hls_thread_local hls::stream<DTYPE> layer3_conv_output_stream("layer3_conv_output_stream");
    hls_thread_local hls::stream<DTYPE> layer4_maxp_output_stream("layer4_maxp_output_stream");
    hls_thread_local hls::stream<DTYPE> layer5_conv_output_stream("layer5_conv_output_stream");
    hls_thread_local hls::stream<DTYPE> layer6_fc_output_stream("layer6_fc_output_stream");

    hls_thread_local hls::task t1(read_input_data, input, in_stream);

    hls_thread_local hls::task t2(read_conv1_data, conv1_weight, conv1_bias, layer1_conv_weight_stream, layer1_conv_bias_stream);
    hls_thread_local hls::task t3(layer1_conv, in_stream, layer1_conv_weight_stream, layer1_conv_bias_stream, layer1_conv_output_stream);
    hls_thread_local hls::task t4(layer2_maxp, layer1_conv_output_stream, layer2_maxp_output_stream);

    hls_thread_local hls::task t5(read_conv3_data, conv3_weight, conv3_bias, layer3_conv_weight_stream, layer3_conv_bias_stream);
    hls_thread_local hls::task t6(layer3_conv, layer2_maxp_output_stream, layer3_conv_weight_stream, layer3_conv_bias_stream, layer3_conv_output_stream);
    hls_thread_local hls::task t7(layer4_maxp, layer3_conv_output_stream, layer4_maxp_output_stream);

    hls_thread_local hls::task t8(read_conv5_data, conv5_weight, conv5_bias, layer5_conv_weight_stream, layer5_conv_bias_stream);
    hls_thread_local hls::task t9(layer5_conv, layer4_maxp_output_stream, layer5_conv_weight_stream, layer5_conv_bias_stream, layer5_conv_output_stream);

    hls_thread_local hls::task t10(read_full6_data, full6_weight, full6_bias, layer6_fc_weight_stream, layer6_fc_bias_stream);
    hls_thread_local hls::task t11(layer6_fc, layer5_conv_output_stream, layer6_fc_weight_stream, layer6_fc_bias_stream, layer6_fc_output_stream);

    hls_thread_local hls::task t12(read_out_data, out_weight, out_bias, layer7_out_weight_stream, layer7_out_bias_stream);
    hls_thread_local hls::task t13(layer7_out, layer6_fc_output_stream, layer7_out_weight_stream, layer7_out_bias_stream, out_stream);

    hls_thread_local hls::task t14(write_output_data, output, out_stream);
}
