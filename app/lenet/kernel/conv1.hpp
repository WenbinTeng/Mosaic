#ifndef __CONV1_H__
#define __CONV1_H__

#include "config.h"

#include "hls_stream.h"

void conv1(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream,
    const weight_t          weight[OUT_CH][K][K],
    const acc_t             bias[OUT_CH]
);

#endif