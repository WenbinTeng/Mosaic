#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_out_space {

constexpr int OUT_CLASS = 10; // output number

using feature_t = ap_int<32>;

void top_out(
    hls::stream<feature_t>& res_stream,
    feature_t res[OUT_CLASS]
);

}  // namespace top_out_space

#endif