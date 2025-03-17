#include "conv1.hpp"

/**
 *  IN:     32*32
 *  WEIGHT: 6*5*5
 *  BIAS:   6
 *  OUT:    6*28*28
 */
void conv1(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[32][32];
    DTYPE weight_buff[5][5];
    DTYPE bias_buff;

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            in_buff[i][j] = in_stream.read();
        }
    }

    DTYPE sum, tmp;

    for (int co = 0; co < 6; co++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                weight_buff[i][j] = weight_stream.read();
            }
        }
        bias_buff = bias_stream.read();

        for (int h = 0; h < 28; h++) {
            for (int w = 0; w < 28; w++) {
#pragma HLS PIPELINE
                sum = 0;
                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 5; j++) {
                        sum += weight_buff[i][j] * in_buff[h + i][w + j];
                    }
                }
                tmp = sum + bias_buff;
                out_stream.write((tmp > 0.0 ? tmp : 0.0));
            }
        }
    }
}