#include "top_in.hpp"

namespace top_in_space {

void top_in(
    feature_t img[IMG_H][IMG_W],
    hls::stream<in_type>& img_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=img offset=slave bundle=data
#pragma HLS INTERFACE axis port=img_stream

    for (int i = 0; i < IMG_H; i++) {
        for (int j = 0; j < IMG_W; j++) {
            img_stream.write((in_type)img[i][j]);
        }
    }
}

}  // namespace top_in_space