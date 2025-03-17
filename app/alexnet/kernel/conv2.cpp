#include "conv2.hpp"

/**
 *  IN:     96*27*27
 *  WEIGHT: 256*96*5*5
 *  BIAS:   256
 *  OUT:    256*27*27
 */
void conv2(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[5][5];
    DTYPE weight_buff[96][5][5];
    DTYPE bias_buff;

    DTYPE sum, tmp;

    for (int co = 0; co < 256; co++) {
        for (int ci = 0; ci < 96; ci++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    weight_buff[ci][i][j] = weight_stream.read();
                }
            }
        }
        bias_buff = bias_stream.read();

        for (int h = 0; h < 27; h++) {
            for (int w = 0; w < 27; w++) {
                sum = 0;
                for (int ci = 0; ci < 96; ci++) {
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
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