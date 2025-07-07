#ifndef __RASTERIZATION1_H__
#define __RASTERIZATION1_H__

#include "typedef.h"

#include "hls_stream.h"

namespace rasterization1_space {

void rasterization1(
    hls::stream<triangle_2d_t> &triangle_2d_stream,
    hls::stream<triangle_2d_t> &triangle_2d_copy_stream,
    hls::stream<triangle_info_t> &triangle_info_stream
);

}  // namespace rasterization1_space

#endif