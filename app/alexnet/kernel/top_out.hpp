#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "config.h"

#include "hls_stream.h"

namespace top_out_space {

constexpr int OUT_NUM = 1000; // output number

void top_out(
    hls::stream<feature_t>& res_stream,
    feature_t res[OUT_NUM]
);

} // namespace top_out_space

#endif