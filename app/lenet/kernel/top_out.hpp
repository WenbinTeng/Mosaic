#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "config.h"

#include "hls_stream.h"

namespace top_out_space {

constexpr int OUT_NUM = 10; // output number

using out_type = ap_int<80>; // result stream output type

void top_out(
    hls::stream<out_type>& res_stream,
    feature_t res[OUT_NUM]
);

} // namespace top_out_space

#endif