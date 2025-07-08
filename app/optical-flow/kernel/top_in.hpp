#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_in_space {

void top_in(
    frames_t frames[MAX_HEIGHT][MAX_WIDTH],
    hls::stream<input_t> &frame1_stream,
    hls::stream<input_t> &frame2_stream,
    hls::stream<input_t> &frame3_stream_a,
    hls::stream<input_t> &frame3_stream_b,
    hls::stream<input_t> &frame4_stream,
    hls::stream<input_t> &frame5_stream
);

}  // namespace top_in_space

#endif