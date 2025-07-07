#include "grad_z_calc.hpp"

namespace grad_z_calc_space {

void grad_z_calc(
    hls::stream<input_t> &frame1_stream,
    hls::stream<input_t> &frame2_stream,
    hls::stream<input_t> &frame3_stream,
    hls::stream<input_t> &frame4_stream,
    hls::stream<input_t> &frame5_stream,
    hls::stream<pixel_t> &gradient_z_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=frame1_stream
#pragma HLS INTERFACE axis port=frame2_stream
#pragma HLS INTERFACE axis port=frame3_stream
#pragma HLS INTERFACE axis port=frame4_stream
#pragma HLS INTERFACE axis port=frame5_stream
#pragma HLS INTERFACE axis port=gradient_z_stream

    const int GRAD_WEIGHTS[] = {1, -8, 0, 8, -1};
    for (int r = 0; r < MAX_HEIGHT; r++) {
        for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS PIPELINE
            input_t f1 = frame1_stream.read();
            input_t f2 = frame2_stream.read();
            input_t f3 = frame3_stream.read();
            input_t f4 = frame4_stream.read();
            input_t f5 = frame5_stream.read();
            gradient_z_stream.write(((pixel_t)(
                f1 * GRAD_WEIGHTS[0] +
                f2 * GRAD_WEIGHTS[1] +
                f3 * GRAD_WEIGHTS[2] +
                f4 * GRAD_WEIGHTS[3] +
                f5 * GRAD_WEIGHTS[4])) / 12);
        }
    }
}

}  // namespace grad_z_calc_space