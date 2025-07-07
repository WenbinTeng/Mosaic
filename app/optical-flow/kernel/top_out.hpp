#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_out_space {

void top_out(
    hls::stream<velocity_t> &output_stream,
    velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]
);

}  // namespace top_out_space

#endif