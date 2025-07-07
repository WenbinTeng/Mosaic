#include "grad_wgt_y.hpp"

namespace grad_wgt_y_space {

void grad_wgt_y(
    hls::stream<pixel_t> &gradient_x_stream,
    hls::stream<pixel_t> &gradient_y_stream,
    hls::stream<pixel_t> &gradient_z_stream,
    hls::stream<gradient_t> &y_filt_grad_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=gradient_x_stream
#pragma HLS INTERFACE axis port=gradient_y_stream
#pragma HLS INTERFACE axis port=gradient_z_stream
#pragma HLS INTERFACE axis port=y_filt_grad_stream

    xf::cv::LineBuffer<7, MAX_WIDTH, gradient_t> buf;
    const pixel_t GRAD_FILTER[] = {0.0755, 0.133, 0.1869, 0.2903, 0.1869, 0.133, 0.0755};
    for (int r = 0; r < MAX_HEIGHT + 3; r++) {
        for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS PIPELINE
#pragma HLS DEPENDENCE variable=buf inter false
            if (r < MAX_HEIGHT) {
                buf.shift_pixels_up(c);
                gradient_t tmp;
                tmp.x = gradient_x_stream.read();
                tmp.y = gradient_y_stream.read();
                tmp.z = gradient_z_stream.read();
                buf.insert_bottom_row(tmp, c);
            } else {
                buf.shift_pixels_up(c);
                gradient_t tmp;
                tmp.x = 0;
                tmp.y = 0;
                tmp.z = 0;
                buf.insert_bottom_row(tmp, c);
            }

            gradient_t acc;
            acc.x = 0;
            acc.y = 0;
            acc.z = 0;
            if (r >= 6 && r < MAX_HEIGHT) {
                for (int i = 0; i < 7; i++) {
                    acc.x += buf.getval(i, c).x * GRAD_FILTER[i];
                    acc.y += buf.getval(i, c).y * GRAD_FILTER[i];
                    acc.z += buf.getval(i, c).z * GRAD_FILTER[i];
                }
                y_filt_grad_stream.write(acc);
            } else if (r >= 3) {
                y_filt_grad_stream.write(acc);
            }
        }
    }
}

}  // namespace grad_wgt_y_space