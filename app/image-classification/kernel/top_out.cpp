#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<feature_t>& res_stream,
    feature_t res[OUT_CLASS]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=res_stream
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=data

    for (int i = 0; i < OUT_CLASS; i++) {
        feature_t label = res_stream.read();
        res[i] = label;
    }
}

} // namespace top_out_space