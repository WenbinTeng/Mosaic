#ifndef __FC1_H__
#define __FC1_H__

#include "config.h"

#include "hls_stream.h"

constexpr int IN_SIZE   = 16 * 5 * 5;   // 400, input size
constexpr int OUT_SIZE  = 120;          // output size
constexpr int PAR       = 10;           // parallel factor

void fc1(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream
);

#endif