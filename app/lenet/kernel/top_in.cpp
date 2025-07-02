#include "top_in.hpp"

void top_in(
    feature_t img[IMG_H][IMG_W],
    hls::stream<feature_t>& img_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=img offset=slave bundle=data
#pragma HLS INTERFACE axis port=img_stream

    for (int i = 0; i < IMG_H; i++) {
        for (int j = 0; j < IMG_W; j++) {
            img_stream.write(img[i][j]);
        }
    }
}