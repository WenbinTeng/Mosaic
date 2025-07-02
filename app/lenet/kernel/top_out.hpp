#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "config.h"

#include "hls_stream.h"

constexpr int OUT_NUM = 10

void top_out(
    hls::stream<feature_t>& res_stream,
    feature_t res[OUT_NUM]
);

#endif