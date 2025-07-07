#ifndef __GRAD_WGT_X_H__
#define __GRAD_WGT_X_H__

#include "typedef.h"

#include "hls_stream.h"
#include "xf_video_mem.hpp"

namespace grad_wgt_x_space {

void grad_wgt_x(
    hls::stream<gradient_t> &y_filt_grad_stream,
    hls::stream<gradient_t> &filt_grad_stream
);

}  // namespace grad_wgt_x_space

#endif