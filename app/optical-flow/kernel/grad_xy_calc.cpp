#include "grad_xy_calc.hpp"

namespace grad_xy_calc_space {

void grad_xy_calc(
    hls::stream<input_t> &frame_stream,
    hls::stream<pixel_t> &gradient_x_stream,
    hls::stream<pixel_t> &gradient_y_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=frame_stream
#pragma HLS INTERFACE axis port=gradient_x_stream
#pragma HLS INTERFACE axis port=gradient_y_stream

    // our own line buffer
    static pixel_t buf[5][MAX_WIDTH];
#pragma HLS ARRAY_PARTITION variable=buf type=complete dim=1

    // small buffer
    pixel_t small_buf[5];
#pragma HLS ARRAY_PARTITION variable=small_buf type=complete dim=0

    // window buffer
    xf::cv::Window<5, 5, input_t> window;

    const int GRAD_WEIGHTS[] = {1, -8, 0, 8, -1};

    for (int r = 0; r < MAX_HEIGHT + 2; r++) {
        for (int c = 0; c < MAX_WIDTH + 2; c++) {
#pragma HLS PIPELINE
            // read out values from current line buffer
            for (int i = 0; i < 4; i++)
                small_buf[i] = buf[i + 1][c];
            // the new value is either 0 or read from frame
            if (r < MAX_HEIGHT && c < MAX_WIDTH)
                small_buf[4] = (pixel_t)frame_stream.read();
            else if (c < MAX_WIDTH)
                small_buf[4] = 0;
            // update line buffer
            if (r < MAX_HEIGHT && c < MAX_WIDTH) {
                for (int i = 0; i < 4; i++)
                    buf[i][c] = small_buf[i];
                buf[4][c] = small_buf[4];
            } else if (c < MAX_WIDTH) {
                for (int i = 0; i < 4; i++)
                    buf[i][c] = small_buf[i];
                buf[4][c] = small_buf[4];
            }

            // manage window buffer
            if (r < MAX_HEIGHT && c < MAX_WIDTH) {
                window.shift_pixels_left();
                for (int i = 0; i < 5; i++)
                    window.insert_pixel(small_buf[i], i, 4);
            } else {
                window.shift_pixels_left();
                window.insert_pixel(0, 0, 4);
                window.insert_pixel(0, 1, 4);
                window.insert_pixel(0, 2, 4);
                window.insert_pixel(0, 3, 4);
                window.insert_pixel(0, 4, 4);
            }

            // compute gradient
            pixel_t x_grad = 0;
            pixel_t y_grad = 0;
            if (r >= 4 && r < MAX_HEIGHT && c >= 4 && c < MAX_WIDTH) {
                for (int i = 0; i < 5; i++) {
                    x_grad += window.getval(2, i) * GRAD_WEIGHTS[i];
                    y_grad += window.getval(i, 2) * GRAD_WEIGHTS[i];
                }
                gradient_x_stream.write(x_grad / 12);
                gradient_y_stream.write(y_grad / 12);
            } else if (r >= 2 && c >= 2) {
                gradient_x_stream.write(0);
                gradient_y_stream.write(0);
            }
        }
    }
}

}  // namespace grad_xy_calc_space