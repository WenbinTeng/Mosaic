#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_out_space {

void top_out(
    hls::stream<bit8> &frame_stream,
    bit32 output[NUM_FB]
);

} // namespace top_out_space

#endif