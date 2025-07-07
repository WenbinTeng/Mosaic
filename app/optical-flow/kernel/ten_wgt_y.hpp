#ifndef __TEN_WGT_Y_H__
#define __TEN_WGT_Y_H__

#include "typedef.h"

#include "hls_stream.h"
#include "xf_video_mem.hpp"

namespace ten_wgt_y_space {

void ten_wgt_y(
    hls::stream<outer_t> &outer_product_stream,
    hls::stream<tensor_t> &tensor_y_stream
);

}  // namespace ten_wgt_y_space

#endif