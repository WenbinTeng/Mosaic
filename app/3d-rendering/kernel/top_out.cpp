#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<bit8> &frame_stream,
    bit32 output[NUM_FB]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=frame_stream
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=data

    bit32 out_tmp = 0;
    for (bit16 i = 0; i < MAX_X; i++) {
        for (bit16 j = 0; j < MAX_Y; j += 4) {
            for (bit8 k = 0; k < 4; k++) {
#pragma HLS PIPELINE
                bit8 frame = frame_stream.read();
                out_tmp.range(k * 8 + 7, k * 8) = frame;
            }
            output[i * MAX_Y / 4 + j / 4] = out_tmp;
        }
    }
}

} // namespace top_out_space