#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_out_space {

void top_out(
    hls::stream<label_t> &result_stream,
    label_t global_results[NUM_TEST]
);

}  // namespace top_out_space

#endif