#include "top_out.hpp"

void top_out(
    hls::stream<feature_t>& res_stream,
    feature_t res[OUT_NUM]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=res_stream
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=data

    for (int i = 0; i < OUT_NUM; i++) {
        res[i] = res_stream.read();
    }
}