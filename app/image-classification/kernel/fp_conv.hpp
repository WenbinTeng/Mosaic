#ifndef __FP_CONV_H__
#define __FP_CONV_H__

#include <cmath>

#include "typedef.h"

#include "hls_stream.h"

namespace fp_conv_space {

constexpr int IN_CH = 3;                           // input channel
constexpr int IN_H = 32;                           // input height
constexpr int IN_W = 32;                           // input width
constexpr int K = 3;                               // kernel size
constexpr int S = 1;                               // stride size
constexpr int P = 1;                               // padding size
constexpr int OUT_CH = 128;                        // output channel
constexpr int OUT_H = (IN_H - K + 2 * P) / S + 1;  // output height
constexpr int OUT_W = (IN_W - K + 2 * P) / S + 1;  // output width
constexpr int PAR = 32;                            // parallel factor

using feature_t = ap_int<16, 4>;  // feature map type
using weight_t = ap_int<16, 4>;   // weight type
using acc_t = ap_int<32>;         // accumulation type
using threshold_t = feature_t;    // threshold type
using din_t = ap_int<16>;         // data input type
using dout_t = ap_int<PAR>;       // data output type

void fp_conv(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
);

}  // namespace fp_conv_space

#endif