#include "conv4.hpp"

/**
 *  IN:     384*13*13
 *  WEIGHT: 384*384*3*3
 *  BIAS:   384
 *  OUT:    384*13*13
 */
void conv4(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[3][3];
    DTYPE weight_buff[384][3][3];
    DTYPE bias_buff;

    DTYPE sum, tmp;

    for (int co = 0; co < 384; co++) {
        for (int ci = 0; ci < 384; ci++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    weight_buff[ci][i][j] = weight_stream.read();
                }
            }
        }
        bias_buff = bias_stream.read();

        for (int h = 0; h < 13; h++) {
            for (int w = 0; w < 13; w++) {
                sum = 0;
                for (int ci = 0; ci < 384; ci++) {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
#pragma HLS PIPELINE
                            in_buff[i][j] = in_stream.read();
                            sum += weight_buff[ci][i][j] * in_buff[i][j];
                        }
                    }
                }
                tmp = sum + bias_buff;
                out_stream.write((tmp > 0.0) ? tmp : 0.0);
            }
        }
    }
}