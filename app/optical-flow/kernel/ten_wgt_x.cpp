#include "ten_wgt_x.hpp"

namespace ten_wgt_x_space {

void ten_wgt_x(
    hls::stream<tensor_t> &tensor_y_stream,
    hls::stream<tensor_t> &tensor_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=tensor_y_stream
#pragma HLS INTERFACE axis port=tensor_stream

    xf::cv::Window<1, 3, tensor_t> buf;
    const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};
    for (int r = 0; r < MAX_HEIGHT; r++) {
        for (int c = 0; c < MAX_WIDTH + 1; c++) {
#pragma HLS PIPELINE
            buf.shift_pixels_left();
            tensor_t tmp;
            if (c < MAX_WIDTH) {
                tmp = tensor_y_stream.read();
            } else {
                for (int i = 0; i < 6; i++)
                    tmp.val[i] = 0;
            }
            buf.insert_pixel(tmp, 0, 2);

            tensor_t acc;
            for (int k = 0; k < 6; k++)
                acc.val[k] = 0;
            if (c >= 2 && c < MAX_WIDTH) {
                for (int i = 0; i < 3; i++) {
                    tmp = buf.getval(0, i);
                    for (int component = 0; component < 6; component++) {
                        acc.val[component] += tmp.val[component] * TENSOR_FILTER[i];
                    }
                }
            }
            if (c >= 1) {
                tensor_stream.write(acc);
            }
        }
    }
}

}  // namespace ten_wgt_x_space