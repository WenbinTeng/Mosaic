#include "ten_wgt_y.hpp"

namespace ten_wgt_y_space {

void ten_wgt_y(
    hls::stream<outer_t> &outer_product_stream,
    hls::stream<tensor_t> &tensor_y_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=outer_product_stream
#pragma HLS INTERFACE axis port=tensor_y_stream

    xf::cv::LineBuffer<3, MAX_WIDTH, outer_t> buf;
    const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};
    for (int r = 0; r < MAX_HEIGHT + 1; r++) {
        for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS PIPELINE
            outer_t tmp;
            buf.shift_pixels_up(c);
            if (r < MAX_HEIGHT) {
                tmp = outer_product_stream.read();
            } else {
                for (int i = 0; i < 6; i++)
                    tmp.val[i] = 0;
            }
            buf.insert_bottom_row(tmp, c);

            tensor_t acc;
            for (int k = 0; k < 6; k++)
                acc.val[k] = 0;

            if (r >= 2 && r < MAX_HEIGHT) {
                for (int i = 0; i < 3; i++) {
                    tmp = buf.getval(i, c);
                    pixel_t k = TENSOR_FILTER[i];
                    for (int component = 0; component < 6; component++) {
                        acc.val[component] += tmp.val[component] * k;
                    }
                }
            }
            if (r >= 1) {
                tensor_y_stream.write(acc);
            }
        }
    }
}

}  // namespace ten_wgt_y_space