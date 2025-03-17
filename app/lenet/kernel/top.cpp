#include "top.hpp"

void read_input_data(DTYPE input[32][32], hls::stream<DTYPE>& in_stream) {
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            in_stream.write(input[i][j]);
        }
    }
}

void read_layer1_conv_data(DTYPE weight[6][5][5], DTYPE bias[6],
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

void read_layer3_conv_data(DTYPE weight[16][6][5][5], DTYPE bias[16],
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

void read_layer5_conv_data(DTYPE weight[120][16][5][5], DTYPE bias[120],
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

void read_layer6_fc_data(DTYPE weight[84][120], DTYPE bias[84],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 84; c++) {
        for (int n = 0; n < 120; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void read_layer7_out_data(DTYPE weight[10][84], DTYPE bias[10],
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
    DTYPE layer1_conv_weight[6][5][5],
    DTYPE layer1_conv_bias[6],
    DTYPE layer3_conv_weight[16][6][5][5],
    DTYPE layer3_conv_bias[16],
    DTYPE layer5_conv_weight[120][16][5][5],
    DTYPE layer5_conv_bias[120],
    DTYPE layer6_fc_weight[84][120],
    DTYPE layer6_fc_bias[84],
    DTYPE layer7_out_weight[10][84],
    DTYPE layer7_out_bias[10],
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

    hls_thread_local hls::task t2(read_layer1_conv_data, layer1_conv_weight, layer1_conv_bias, layer1_conv_weight_stream, layer1_conv_bias_stream);
    hls_thread_local hls::task t3(layer1_conv, in_stream, layer1_conv_weight_stream, layer1_conv_bias_stream, layer1_conv_output_stream);
    hls_thread_local hls::task t4(layer2_maxp, layer1_conv_output_stream, layer2_maxp_output_stream);

    hls_thread_local hls::task t5(read_layer3_conv_data, layer3_conv_weight, layer3_conv_bias, layer3_conv_weight_stream, layer3_conv_bias_stream);
    hls_thread_local hls::task t6(layer3_conv, layer2_maxp_output_stream, layer3_conv_weight_stream, layer3_conv_bias_stream, layer3_conv_output_stream);
    hls_thread_local hls::task t7(layer4_maxp, layer3_conv_output_stream, layer4_maxp_output_stream);

    hls_thread_local hls::task t8(read_layer5_conv_data, layer5_conv_weight, layer5_conv_bias, layer5_conv_weight_stream, layer5_conv_bias_stream);
    hls_thread_local hls::task t9(layer5_conv, layer4_maxp_output_stream, layer5_conv_weight_stream, layer5_conv_bias_stream, layer5_conv_output_stream);

    hls_thread_local hls::task t10(read_layer6_fc_data, layer6_fc_weight, layer6_fc_bias, layer6_fc_weight_stream, layer6_fc_bias_stream);
    hls_thread_local hls::task t11(layer6_fc, layer5_conv_output_stream, layer6_fc_weight_stream, layer6_fc_bias_stream, layer6_fc_output_stream);

    hls_thread_local hls::task t12(read_layer7_out_data, layer7_out_weight, layer7_out_bias, layer7_out_weight_stream, layer7_out_bias_stream);
    hls_thread_local hls::task t13(layer7_out, layer6_fc_output_stream, layer7_out_weight_stream, layer7_out_bias_stream, out_stream);

    hls_thread_local hls::task t14(write_output_data, output, out_stream);
}
