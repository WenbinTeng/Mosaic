#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<velocity_t> &output_stream,
    velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=output_stream
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=data

    for (int r = 0; r < MAX_HEIGHT; r++) {
        for (int c = 0; c < MAX_WIDTH; c++) {
            velocity_t v = output_stream.read();
            outputs[r][c] = v;
        }
    }
}

}  // namespace top_out_space