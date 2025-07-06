#ifndef __CONV1_H__
#define __CONV1_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace conv1_space {

constexpr int IN_CH = 1;            // input channel
constexpr int IN_H = 32;            // input height
constexpr int IN_W = 32;            // input width
constexpr int K = 5;                // kernel size
constexpr int OUT_CH = 6;           // output channel
constexpr int OUT_H = IN_H - K + 1; // 28, output height
constexpr int OUT_W = IN_W - K + 1; // 28, output width

using din_t = ap_int<IN_CH * 8>;    // data input type
using dout_t = ap_int<OUT_CH * 8>;  // data output type

void conv1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace conv1_space

#endif