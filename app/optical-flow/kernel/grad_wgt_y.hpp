#ifndef __GRAD_WGT_Y_H__
#define __GRAD_WGT_Y_H__

#include "typedef.h"

#include "hls_stream.h"
#include "xf_video_mem.hpp"

namespace grad_wgt_y_space {

void grad_wgt_y(
    hls::stream<pixel_t> &gradient_x_stream,
    hls::stream<pixel_t> &gradient_y_stream,
    hls::stream<pixel_t> &gradient_z_stream,
    hls::stream<gradient_t> &y_filt_grad_stream
);

}  // namespace grad_wgt_y_space

#endif