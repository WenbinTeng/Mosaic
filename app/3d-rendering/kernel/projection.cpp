#include "projection.hpp"

namespace projection_space {

void projection(
    hls::stream<triangle_3d_t> &triangle_3d_stream,
    hls::stream<triangle_2d_t> &triangle_2d_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=triangle_3d_stream
#pragma HLS INTERFACE axis port=triangle_2d_stream

    triangle_3d_t t3d = triangle_3d_stream.read();
    triangle_2d_t t2d;
    t2d.x0 = t3d.x0;
    t2d.y0 = t3d.y0;
    t2d.x1 = t3d.x1;
    t2d.y1 = t3d.y1;
    t2d.x2 = t3d.x2;
    t2d.y2 = t3d.y2;
    t2d.z = t3d.z0 / 3 + t3d.z1 / 3 + t3d.z2 / 3;
    triangle_2d_stream.write(t2d);
}

}  // namespace projection_space