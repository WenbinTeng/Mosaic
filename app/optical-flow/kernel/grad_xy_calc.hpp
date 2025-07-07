#ifndef __GRAD_XY_CALC_H__
#define __GRAD_XY_CALC_H__

#include "typedef.h"

#include "hls_stream.h"
#include "xf_video_mem.hpp"

namespace grad_xy_calc_space {

void grad_xy_calc(
    hls::stream<input_t> &frame_stream,
    hls::stream<pixel_t> &gradient_x_stream,
    hls::stream<pixel_t> &gradient_y_stream
);

}  // namespace grad_xy_calc_space

#endif