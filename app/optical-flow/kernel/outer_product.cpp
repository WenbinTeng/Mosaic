#include "outer_product.hpp"

namespace outer_product_space {

void outer_product(
    hls::stream<gradient_t> &gradient_stream,
    hls::stream<outer_t> &outer_product_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=gradient_stream
#pragma HLS INTERFACE axis port=outer_product_stream

    for (int r = 0; r < MAX_HEIGHT; r++) {
        for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS PIPELINE
            gradient_t grad = gradient_stream.read();
            outer_pixel_t x = (outer_pixel_t)grad.x;
            outer_pixel_t y = (outer_pixel_t)grad.y;
            outer_pixel_t z = (outer_pixel_t)grad.z;
            outer_t out;
            out.val[0] = (x * x);
            out.val[1] = (y * y);
            out.val[2] = (z * z);
            out.val[3] = (x * y);
            out.val[4] = (x * z);
            out.val[5] = (y * z);
            outer_product_stream.write(out);
        }
    }
}

}  // namespace outer_product_space