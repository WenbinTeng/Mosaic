#include "rasterization2.hpp"

namespace rasterization2_space {

// Given a pixel, determine whether it is inside the triangle
// by Pineda algorithm
// if so, return true
// else, return false
bit1 _pixel_in_triangle(bit8 x, bit8 y, triangle_2d_t &t2d) {
#pragma HLS INLINE
    bit32 pi0, pi1, pi2;
    pi0 = (x - t2d.x0) * (t2d.y1 - t2d.y0) - (y - t2d.y0) * (t2d.x1 - t2d.x0);
    pi1 = (x - t2d.x1) * (t2d.y2 - t2d.y1) - (y - t2d.y1) * (t2d.x2 - t2d.x1);
    pi2 = (x - t2d.x2) * (t2d.y0 - t2d.y2) - (y - t2d.y2) * (t2d.x0 - t2d.x2);
    return (pi0 >= 0 && pi1 >= 0 && pi2 >= 0);
}

void rasterization2(
    hls::stream<triangle_2d_t> &triangle_2d_copy_stream,
    hls::stream<triangle_info_t> &triangle_info_stream,
    hls::stream<fragment_t> &fragment_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=triangle_2d_copy_stream
#pragma HLS INTERFACE axis port=triangle_info_stream
#pragma HLS INTERFACE axis port=fragment_stream

    triangle_2d_t t2d = triangle_2d_copy_stream.read();
    triangle_info_t info = triangle_info_stream.read();

    bit8 color = 100;
    for (bit16 k = 0; k < info.box; k++) {
#pragma HLS PIPELINE
        fragment_t frag;
        bit8 x = info.minX + k % info.width;
        bit8 y = info.minY + k / info.width;
        if (_pixel_in_triangle(x, y, t2d)) {
            frag.x = x;
            frag.y = y;
            frag.z = t2d.z;
            frag.color = color;
            fragment_stream.write(frag);
        }
    }
}

}  // namespace rasterization2_space