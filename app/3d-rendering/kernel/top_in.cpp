#include "top_in.hpp"

namespace top_in_space {

void top_in(
    bit32 input[3 * NUM_3D_TRI],
    hls::stream<triangle_3d_t> &triangle_3d_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=data
#pragma HLS INTERFACE axis port=triangle_3d_stream

    static bit16 index = 0;

    triangle_3d_t t3d;
    bit32 input_lo = input[3 * index];
    bit32 input_mi = input[3 * index + 1];
    bit32 input_hi = input[3 * index + 2];
    t3d.x0 = input_lo.range( 7,  0);
    t3d.y0 = input_lo.range(15,  8);
    t3d.z0 = input_lo.range(23, 16);
    t3d.x1 = input_lo.range(31, 24);
    t3d.y1 = input_mi.range( 7,  0);
    t3d.z1 = input_mi.range(15,  8);
    t3d.x2 = input_mi.range(23, 16);
    t3d.y2 = input_mi.range(31, 24);
    t3d.z2 = input_hi.range( 7,  0);
    triangle_3d_stream.write(t3d);

    index = (index + 1) % NUM_3D_TRI;
}

}  // namespace top_in_space