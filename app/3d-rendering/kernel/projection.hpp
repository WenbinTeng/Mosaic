#ifndef __PROJECTION_H__
#define __PROJECTION_H__

#include "typedef.h"

#include "hls_stream.h"

namespace projection_space {

void projection(
    hls::stream<triangle_3d_t> &triangle_3d_stream,
    hls::stream<triangle_2d_t> &triangle_2d_stream
);

}  // namespace projection_space

#endif