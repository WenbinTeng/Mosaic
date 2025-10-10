#ifndef __CONV2_H__
#define __CONV2_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace conv2_space {

constexpr int PREV_PAR = 4;                       // previous parallel factor
constexpr int IN_CH = 96;                         // input channel
constexpr int IN_H = 27;                          // input height
constexpr int IN_W = 27;                          // input width
constexpr int K = 5;                              // kernel size
constexpr int S = 1;                              // stride size
constexpr int P = 2;                              // padding size
constexpr int OUT_CH = 256;                       // output channel
constexpr int OUT_H = (IN_H - K + 2 * P) / S + 1; // 27, output height
constexpr int OUT_W = (IN_W - K + 2 * P) / S + 1; // 27, output width
constexpr int PAR = 8;                            // parallel factor

using din_t = ap_int<PREV_PAR * 8>; // data input type
using dout_t = ap_int<PAR * 8>;     // data output type

void conv2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace conv2_space

#endif