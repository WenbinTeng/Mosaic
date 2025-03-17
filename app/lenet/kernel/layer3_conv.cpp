#include "layer3_conv.hpp"

/**
 *  IN:     16*5*5
 *  WEIGHT: 120*16*5*5
 *  BIAS:   120
 *  OUT:    120
 */
void layer3_conv(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[16][5][5];
    DTYPE weight_buff[16][5][5];
    DTYPE bias_buff;

    for (int ci = 0; ci < 16; ci++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                in_buff[ci][i][j] = in_stream.read();
            }
        }
    }

    DTYPE sum, tmp;

    for (int co = 0; co < 120; co++) {
        for (int ci = 0; ci < 16; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    weight_buff[ci][i][j] = weight_stream.read();
                }
            }
        }
        bias_buff = bias_stream.read();

        sum = 0;
        for (int ci = 0; ci < 16; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
#pragma HLS PIPELINE
                    sum += weight_buff[ci][i][j] * in_buff[ci][i][j];
                }
            }
        }
        tmp = sum + bias_buff;
        out_stream.write((tmp > 0.0) ? tmp : 0.0);
    }
}