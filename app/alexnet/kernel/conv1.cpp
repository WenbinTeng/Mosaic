#include "conv1.hpp"

/**
 *  IN:     3*224*224
 *  WEIGHT: 96*3*11*11
 *  BIAS:   96
 *  OUT:    96*55*55
 */
void conv1(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[11];
    DTYPE weight_buff[3][11][11];
    DTYPE bias_buff;

    DTYPE sum, tmp;

    for (int co = 0; co < 96; co++) {
        for (int ci = 0; ci < 3; ci++) {
            for (int i = 0; i < 11; i++) {
                for (int j = 0; j < 11; j++) {
                    weight_buff[ci][i][j] = weight_stream.read();
                }
            }
        }
        bias_buff = bias_stream.read();

        for (int h = 0; h < 55; h++) {
            for (int w = 0; w < 55; w++) {
                int ch = h * 4;
                int cw = w * 4;
                sum = 0;
                for (int ci = 0; ci < 3; ci++) {
                    for (int i = 0; i < 11; i++) {
                        for (int j = 0; j < 11; j++) {
#pragma HLS PIPELINE
                            in_buff[j] = in_stream.read();
                            sum += weight_buff[ci][i][j] * in_buff[j];
                        }
                    }
                }
                tmp = sum + bias_buff;
                out_stream.write((tmp > 0.0) ? tmp : 0.0);
            }
        }
    }
}