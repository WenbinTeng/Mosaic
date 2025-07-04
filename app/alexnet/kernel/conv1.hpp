#ifndef __CONV1_H__
#define __CONV1_H__

#include "config.h"

#include "hls_stream.h"

namespace conv1_space {
constexpr int IN_CH = 3;                           // input channel
constexpr int IN_H = 227;                          // input height
constexpr int IN_W = 227;                          // input width
constexpr int K = 11;                              // kernel size
constexpr int S = 4;                               // stride size
constexpr int P = 0;                               // padding size
constexpr int OUT_CH = 96;                         // output channel
constexpr int OUT_H = (IN_H - K + 2 * P) / S + 1;  // 55, output height
constexpr int OUT_W = (IN_W - K + 2 * P) / S + 1;  // 55, output width
constexpr int PAR = 16;                            // parallel factor
using din_t = ap_int<IN_CH * 8>;                   // data input type
using dout_t = ap_int<PAR * 8>;                    // data output type
}  // namespace conv1_space

void conv1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

#endif