#include "maxp1.hpp"

/**
 *  IN:     96*55*55
 *  OUT:    96*27*27
 */
void maxp1(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[55][55];
    DTYPE data;
    bool cond;

    for (int c = 0; c < 96; c++) {
        for (int i = 0; i < 55; i++) {
            for (int j = 0; j < 55; j++) {
                in_buff[i][j] = in_stream.read();
            }
        }

        for (int h = 0; h < 27; h++) {
            for (int w = 0; w < 27; w++) {
                DTYPE max_value = -1000000000000.0;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
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