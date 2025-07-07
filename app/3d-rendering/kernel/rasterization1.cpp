#include "rasterization1.hpp"

namespace rasterization1_space {

// Determine whether three vertices of a trianlgLe
// (x0,y0) (x1,y1) (x2,y2) are in clockwise order by Pineda algorithm
// if so, return a number > 0
// else if three points are in line, return a number == 0
// else in counterclockwise order, return a number < 0
int _check_clockwise(triangle_2d_t &t2d) {
#pragma HLS INLINE
    int cw;
    cw = (t2d.x2 - t2d.x0) * (t2d.y1 - t2d.y0) -
         (t2d.y2 - t2d.y0) * (t2d.x1 - t2d.x0);
    return cw;
}

// swap (x0, y0) (x1, y1) of a Triangle_2D
void _clockwise_vertices(triangle_2d_t &t2d) {
#pragma HLS INLINE
    bit8 tmp_x, tmp_y;
    tmp_x = t2d.x0;
    tmp_y = t2d.y0;
    t2d.x0 = t2d.x1;
    t2d.y0 = t2d.y1;
    t2d.x1 = tmp_x;
    t2d.y1 = tmp_y;
}

bit8 _min3(bit8 in0, bit8 in1, bit8 in2) {
#pragma HLS INLINE
    bit8 m = (in0 < in1) ? in0 : in1;
    return (m < in2) ? m : in2;
}

bit8 _max3(bit8 in0, bit8 in1, bit8 in2) {
#pragma HLS INLINE
    bit8 m = (in0 > in1) ? in0 : in1;
    return (m > in2) ? m : in2;
}

void rasterization1(
    hls::stream<triangle_2d_t> &triangle_2d_stream,
    hls::stream<triangle_2d_t> &triangle_2d_copy_stream,
    hls::stream<triangle_info_t> &triangle_info_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=triangle_2d_stream
#pragma HLS INTERFACE axis port=triangle_2d_copy_stream
#pragma HLS INTERFACE axis port=triangle_info_stream

    triangle_2d_t t2d = triangle_2d_stream.read();

    triangle_2d_t t2d_copy = t2d;
    triangle_2d_copy_stream.write(t2d_copy);

    triangle_info_t info;
    if (_check_clockwise(t2d) == 0)
        return;
    if (_check_clockwise(t2d) <  0)
        _clockwise_vertices(t2d);
    info.minX = _min3(t2d.x0, t2d.x1, t2d.x2);
    info.maxX = _max3(t2d.x0, t2d.x1, t2d.x2);
    info.minY = _min3(t2d.y0, t2d.y1, t2d.y2);
    info.maxY = _max3(t2d.y0, t2d.y1, t2d.y2);
    info.width = info.maxX - info.minX;
    info.box = (info.maxX - info.minX) * (info.maxY - info.minY);
    info.flag = 0;
    triangle_info_stream.write(info);
}

}  // namespace rasterization1_space