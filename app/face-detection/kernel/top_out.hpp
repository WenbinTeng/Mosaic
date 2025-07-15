#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_out_space {

void top_out(
    hls::stream<result_t> &result_stream,
    int result_x[RESULT_SIZE],
    int result_y[RESULT_SIZE],
    int result_w[RESULT_SIZE],
    int result_h[RESULT_SIZE],
    int *result_size
);

}  // namespace top_out_space

#endif