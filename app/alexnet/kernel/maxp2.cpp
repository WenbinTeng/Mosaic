#include "maxp2.hpp"

/**
 *  IN:     256*27*27
 *  OUT:    256*13*13
 */
void maxp2(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[27][27];
    DTYPE data;
    bool cond;

    for (int c = 0; c < 256; c++) {
        for (int i = 0; i < 27; i++) {
            for (int j = 0; j < 27; j++) {
                in_buff[i][j] = in_stream.read();
            }
        }

        for (int h = 0; h < 13; h++) {
            for (int w = 0; w < 13; w++) {
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