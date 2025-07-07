#ifndef __TEN_WGT_X_H__
#define __TEN_WGT_X_H__

#include "typedef.h"

#include "hls_stream.h"
#include "xf_video_mem.hpp"

namespace ten_wgt_x_space {

void ten_wgt_x(
    hls::stream<tensor_t> &tensor_y_stream,
    hls::stream<tensor_t> &tensor_stream
);

}  // namespace ten_wgt_x_space

#endif