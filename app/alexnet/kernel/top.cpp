#include "top.hpp"

void read_input_data(DTYPE input[3][224][224], hls::stream<DTYPE>& in_stream) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < 224; i++) {
            for (int j = 0; j < 224; j++) {
                in_stream.write(input[c][i][j]);
            }
        }
    }
}

void read_conv1_data(DTYPE weight[96][3][11][11], DTYPE bias[96],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int co = 0; co < 96; co++) {
        for (int ci = 0; ci < 3; ci++) {
            for (int i = 0; i < 11; i++) {
                for (int j = 0; j < 11; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_conv2_data(DTYPE weight[256][96][5][5], DTYPE bias[256],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int co = 0; co < 256; co++) {
        for (int ci = 0; ci < 96; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_conv3_data(DTYPE weight[384][256][3][3], DTYPE bias[384],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int co = 0; co < 384; co++) {
        for (int ci = 0; ci < 256; ci++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_conv4_data(DTYPE weight[384][384][3][3], DTYPE bias[384],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int co = 0; co < 384; co++) {
        for (int ci = 0; ci < 384; ci++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_conv5_data(DTYPE weight[256][384][3][3], DTYPE bias[256],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int co = 0; co < 256; co++) {
        for (int ci = 0; ci < 384; ci++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    weight_stream.write(weight[co][ci][i][j]);
                }
            }
        }
        bias_stream.write(bias[co]);
    }
}

void read_full6_data(DTYPE weight[4096][9216], DTYPE bias[4096],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 4096; c++) {
        for (int n = 0; n < 9216; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void read_full7_data(DTYPE weight[4096][4096], DTYPE bias[4096],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 4096; c++) {
        for (int n = 0; n < 4096; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void read_full8_data(DTYPE weight[1000][4096], DTYPE bias[1000],
                     hls::stream<DTYPE>& weight_stream,
                     hls::stream<DTYPE>& bias_stream) {
    for (int c = 0; c < 1000; c++) {
        for (int n = 0; n < 4096; n++) {
            weight_stream.write(weight[c][n]);
        }
        bias_stream.write(bias[c]);
    }
}

void write_output_data(DTYPE output[1000], hls::stream<DTYPE>& out_stream) {
    for (int n = 0; n < 1000; n++) {
        output[n] = out_stream.read();
    }
}

void top(
    DTYPE input[3][224][224],
    DTYPE conv1_weight[96][3][11][11],
    DTYPE conv1_bias[96],
    DTYPE conv2_weight[256][96][5][5],
    DTYPE conv2_bias[256],
    DTYPE conv3_weight[384][256][3][3],
    DTYPE conv3_bias[384],
    DTYPE conv4_weight[384][384][3][3],
    DTYPE conv4_bias[384],
    DTYPE conv5_weight[256][384][3][3],
    DTYPE conv5_bias[256],
    DTYPE full6_weight[4096][9216],
    DTYPE full6_bias[4096],
    DTYPE full7_weight[4096][4096],
    DTYPE full7_bias[4096],
    DTYPE full8_weight[1000][4096],
    DTYPE full8_bias[1000],
    DTYPE output[1000]
) {
    hls_thread_local hls::stream<DTYPE> in_stream("in_stream");
    hls_thread_local hls::stream<DTYPE> conv1_weight_stream("conv1_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv1_bias_stream("conv1_bias_stream");
    hls_thread_local hls::stream<DTYPE> conv2_weight_stream("conv2_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv2_bias_stream("conv2_bias_stream");
    hls_thread_local hls::stream<DTYPE> conv3_weight_stream("conv3_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv3_bias_stream("conv3_bias_stream");
    hls_thread_local hls::stream<DTYPE> conv4_weight_stream("conv4_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv4_bias_stream("conv4_bias_stream");
    hls_thread_local hls::stream<DTYPE> conv5_weight_stream("conv5_weight_stream");
    hls_thread_local hls::stream<DTYPE> conv5_bias_stream("conv5_bias_stream");
    hls_thread_local hls::stream<DTYPE> full6_weight_stream("full6_weight_stream");
    hls_thread_local hls::stream<DTYPE> full6_bias_stream("full6_bias_stream");
    hls_thread_local hls::stream<DTYPE> full7_weight_stream("full7_weight_stream");
    hls_thread_local hls::stream<DTYPE> full7_bias_stream("full7_bias_stream");
    hls_thread_local hls::stream<DTYPE> full8_weight_stream("full8_weight_stream");
    hls_thread_local hls::stream<DTYPE> full8_bias_stream("full8_bias_stream");
    hls_thread_local hls::stream<DTYPE> out_stream("out_stream");

    hls_thread_local hls::stream<DTYPE> conv1_out_stream("conv1_out_stream");
    hls_thread_local hls::stream<DTYPE> maxp1_out_stream("maxp1_out_stream");
    hls_thread_local hls::stream<DTYPE> conv2_out_stream("conv2_out_stream");
    hls_thread_local hls::stream<DTYPE> maxp2_out_stream("maxp2_out_stream");
    hls_thread_local hls::stream<DTYPE> conv3_out_stream("conv3_out_stream");
    hls_thread_local hls::stream<DTYPE> conv4_out_stream("conv4_out_stream");
    hls_thread_local hls::stream<DTYPE> conv5_out_stream("conv5_out_stream");
    hls_thread_local hls::stream<DTYPE> maxp5_out_stream("maxp5_out_stream");
    hls_thread_local hls::stream<DTYPE> full6_out_stream("full6_out_stream");
    hls_thread_local hls::stream<DTYPE> full7_out_stream("full7_out_stream");

    hls_thread_local hls::task t1(read_input_data, input, in_stream);

    hls_thread_local hls::task t2(read_conv1_data, conv1_weight, conv1_bias, conv1_weight_stream, conv1_bias_stream);
    hls_thread_local hls::task t3(conv1, in_stream, conv1_weight_stream, conv1_bias_stream, conv1_out_stream);
    hls_thread_local hls::task t4(maxp1, conv1_out_stream, maxp1_out_stream);

    hls_thread_local hls::task t5(read_conv2_data, conv2_weight, conv2_bias, conv2_weight_stream, conv2_bias_stream);
    hls_thread_local hls::task t6(conv2, maxp1_out_stream, conv2_weight_stream, conv2_bias_stream, conv2_out_stream);
    hls_thread_local hls::task t7(maxp2, conv2_out_stream, maxp2_out_stream);

    hls_thread_local hls::task t8(read_conv3_data, conv3_weight, conv3_bias, conv3_weight_stream, conv3_bias_stream);
    hls_thread_local hls::task t9(conv3, maxp2_out_stream, conv3_weight_stream, conv3_bias_stream, conv3_out_stream);

    hls_thread_local hls::task t10(read_conv4_data, conv4_weight, conv4_bias, conv4_weight_stream, conv4_bias_stream);
    hls_thread_local hls::task t11(conv4, conv3_out_stream, conv4_weight_stream, conv4_bias_stream, conv4_out_stream);

    hls_thread_local hls::task t12(read_conv5_data, conv5_weight, conv5_bias, conv5_weight_stream, conv5_bias_stream);
    hls_thread_local hls::task t13(conv5, conv4_out_stream, conv5_weight_stream, conv5_bias_stream, conv5_out_stream);
    hls_thread_local hls::task t14(maxp5, conv5_out_stream, maxp5_out_stream);

    hls_thread_local hls::task t15(read_full6_data, full6_weight, full6_bias, full6_weight_stream, full6_bias_stream);
    hls_thread_local hls::task t16(full6, maxp5_out_stream, full6_weight_stream, full6_bias_stream, full6_out_stream);

    hls_thread_local hls::task t17(read_full7_data, full7_weight, full7_bias, full7_weight_stream, full7_bias_stream);
    hls_thread_local hls::task t18(full7, full6_out_stream, full7_weight_stream, full7_bias_stream, full7_out_stream);

    hls_thread_local hls::task t19(read_full8_data, full8_weight, full8_bias, full8_weight_stream, full8_bias_stream);
    hls_thread_local hls::task t20(full8, full7_out_stream, full8_weight_stream, full8_bias_stream, out_stream);

    hls_thread_local hls::task t21(write_output_data, output, out_stream);
}
