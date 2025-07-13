#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<label_t> &result_stream,
    label_t global_results[NUM_TEST]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=result_stream
#pragma HLS INTERFACE m_axi port=global_results offset=slave bundle=data

    for (int i = 0; i < NUM_TEST; i++) {
#pragma HLS PIPELINE
        label_t res = result_stream.read();
        global_results[i] = res;
    }
}

}  // namespace top_out_space