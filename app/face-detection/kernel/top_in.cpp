#include "top_in.hpp"

namespace top_in_space {

void top_in(
    data_t data[IMAGE_WIDTH],
    hls::stream<data_t> &pixel_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=data
#pragma HLS INTERFACE axis port=pixel_stream

    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
#pragma HLS PIPELINE
            data_t d = data[j];
            pixel_stream.write(d);
        }
    }
}

}  // namespace top_in_space