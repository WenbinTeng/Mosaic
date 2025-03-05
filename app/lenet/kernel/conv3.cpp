#include "conv3.hpp"

/**
 *  IN:     6*14*14
 *  WEIGHT: 16*6*5*5
 *  BIAS:   16
 *  OUT:    16*10*10
 */
void conv3(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
) {
    float in_buff[6][14][14];
    float weight_buff[6][5][5];
    float bias_buff;

    for (int ci = 0; ci < 6; ci++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                in_buff[ci][i][j] = in_stream.read();
            }
        }
    }

    float sum, tmp;

    for (int co = 0; co < 16; co++) {
        for (int ci = 0; ci < 6; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    weight_buff[ci][i][j] = weight_stream.read();
                }
            }
        }
        bias_buff = bias_stream.read();

        for (int h = 0; h < 10; h++) {
            for (int w = 0; w < 10; w++) {
                sum = 0;
                for (int ci = 0; ci < 6; ci++) {
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
#pragma HLS PIPELINE
                            sum += weight_buff[ci][i][j] * in_buff[ci][h + i][w + j];
                        }
                    }
                }
                tmp = sum + bias_buff;
                out_stream.write((tmp > 0.0) ? tmp : 0.0);
            }
        }
    }
}