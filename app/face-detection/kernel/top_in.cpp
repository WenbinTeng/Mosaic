#include "top_in.hpp"

namespace top_in_space {

void top_in(
    data_t data[IMAGE_WIDTH],
    hls::stream<data_t> &pixel_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=data
#pragma HLS INTERFACE axis port=pixel_stream

}

}  // namespace top_in_space