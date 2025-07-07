#ifndef __RASTERIZATION2_H__
#define __RASTERIZATION2_H__

#include "typedef.h"

#include "hls_stream.h"

namespace rasterization2_space {

void rasterization2(
    hls::stream<triangle_2d_t> &triangle_2d_copy_stream,
    hls::stream<triangle_info_t> &triangle_info_stream,
    hls::stream<fragment_t> &fragment_stream
);

}  // namespace rasterization2_space

#endif