#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_in_space {

void top_in(
    bit32 input[3 * NUM_3D_TRI],
    hls::stream<triangle_3d_t> &triangle_3d_stream
);

}  // namespace top_in_space

#endif