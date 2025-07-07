#include "top_in.hpp"

namespace top_in_space {

void top_in(
    frames_t frames[MAX_HEIGHT][MAX_WIDTH],
    hls::stream<input_t> frame1_stream,
    hls::stream<input_t> frame2_stream,
    hls::stream<input_t> frame3_stream_a,
    hls::stream<input_t> frame3_stream_b,
    hls::stream<input_t> frame4_stream,
    hls::stream<input_t> frame5_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=frames offset=slave bundle=data
#pragma HLS INTERFACE axis port=frame1_stream
#pragma HLS INTERFACE axis port=frame2_stream
#pragma HLS INTERFACE axis port=frame3_stream_a
#pragma HLS INTERFACE axis port=frame3_stream_b
#pragma HLS INTERFACE axis port=frame4_stream
#pragma HLS INTERFACE axis port=frame5_stream

    frames_t buf;
    for (int r = 0; r < MAX_HEIGHT; r++) {
        for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS PIPELINE
            // one wide read
            buf = frames[r][c];
            // assign values to the FIFOs
            frame1_stream.write((input_t)(buf.range(7, 0)) >> 8);
            frame2_stream.write((input_t)(buf.range(15, 8)) >> 8);
            frame3_stream_a.write((input_t)(buf.range(23, 16)) >> 8);
            frame3_stream_b.write((input_t)(buf.range(23, 16)) >> 8);
            frame4_stream.write((input_t)(buf.range(31, 24)) >> 8);
            frame5_stream.write((input_t)(buf.range(39, 32)) >> 8);
        }
    }
}

}  // namespace top_in_space