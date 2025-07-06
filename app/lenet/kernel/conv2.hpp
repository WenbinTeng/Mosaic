#ifndef __CONV2_H__
#define __CONV2_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace conv2_space {

constexpr int IN_CH = 6;            // input channel
constexpr int IN_H = 14;            // input height
constexpr int IN_W = 14;            // input width
constexpr int K = 5;                // kernel size
constexpr int OUT_CH = 16;          // output channel
constexpr int OUT_H = IN_H - K + 1; // 10, output height
constexpr int OUT_W = IN_W - K + 1; // 10, output width
constexpr int PAR = 16;             // parallel factor

using din_t = ap_int<IN_CH * 8>; // data input type
using dout_t = ap_int<PAR * 8>;  // data output type

void conv2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace conv2_space

#endif