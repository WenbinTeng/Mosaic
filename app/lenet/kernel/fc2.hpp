#ifndef __FC2_H__
#define __FC2_H__

#include "config.h"

#include "hls_stream.h"

void fc2(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream,
    const weight_t          weight[IN_SIZE][OUT_SIZE],
    const acc_t             bias[OUT_SIZE]
);

#endif