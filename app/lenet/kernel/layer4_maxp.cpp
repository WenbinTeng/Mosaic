#include "layer4_maxp.hpp"

/**
 *  IN:     16*10*10
 *  OUT:    16*5*5
 */
void layer4_maxp(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[10][10];
    DTYPE data;
    bool cond;

    for (int c = 0; c < 16; c++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                in_buff[i][j] = in_stream.read();
            }
        }

        for (int h = 0; h < 5; h++) {
            for (int w = 0; w < 5; w++) {
                DTYPE max_value = -1000000000000.0;
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