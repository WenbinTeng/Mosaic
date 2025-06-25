#ifndef __FC1_H__
#define __FC1_H__

#include "config.h"

#include "hls_stream.h"

void fc1(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream,
    const weight_t          weight[IN_SIZE][OUT_SIZE],
    const acc_t             bias[OUT_SIZE]
);

#endif