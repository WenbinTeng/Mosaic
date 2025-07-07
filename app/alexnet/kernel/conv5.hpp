#ifndef __CONV5_H__
#define __CONV5_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace conv5_space {

constexpr int PREV_PAR = 16;                      // previous parallel factor
constexpr int IN_CH = 384;                        // input channel
constexpr int IN_H = 13;                          // input height
constexpr int IN_W = 13;                          // input width
constexpr int K = 3;                              // kernel size
constexpr int S = 1;                              // stride size
constexpr int P = 1;                              // padding size
constexpr int OUT_CH = 256;                       // output channel
constexpr int OUT_H = (IN_H - K + 2 * P) / S + 1; // 13, output height
constexpr int OUT_W = (IN_W - K + 2 * P) / S + 1; // 13, output width
constexpr int PAR = 16;                           // parallel factor

using din_t = ap_int<PREV_PAR * 8>; // data input type
using dout_t = ap_int<PAR * 8>;     // data output type

void conv5(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace conv5_space

#endif