#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<out_type>& res_stream,
    feature_t res[OUT_NUM]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=res_stream
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=data

    for (int i = 0; i < OUT_NUM; i++) {
        out_type d = res_stream.read();
        for (int j = 0; j < 10; j++) {
            res[i * 8 + j] = d.range(j * 8 + 7, j * 8);
        }
    }
}

} // namespace top_out_space