#include "samp2.hpp"

/**
 *  IN:     6*28*28
 *  OUT:    6*14*14
 */
void samp2(
    hls::stream<float>& in_stream,
    hls::stream<float>& out_stream
) {
    float in_buff[28][28];
    float data;
    bool cond;

    for (int c = 0; c < 6; c++) {
        for (int i = 0; i < 28; i++) {
            for (int j = 0; j < 28; j++) {
                in_buff[i][j] = in_stream.read();
            }
        }

        for (int h = 0; h < 14; h++) {
            for (int w = 0; w < 14; w++) {
                float max_value = -10000000.0;
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
#pragma HLS PIPELINE
                        data = in_buff[(h * 2) + i][(w * 2) + j];
                        cond = (max_value > data);
                        max_value = cond ? max_value : data;
                    }
                }
                out_stream.write((max_value > 0.0) ? max_value : 0.0);
            }
        }
    }
}