#ifndef __FC1_H__
#define __FC1_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace fc1_space {

constexpr int PREV_PAR = 16;         // previous parallel factor
constexpr int IN_SIZE = 256 * 6 * 6; // 9216, input size
constexpr int OUT_SIZE = 4096;       // output size
constexpr int PAR = 16;              // parallel factor

using din_t = ap_int<PREV_PAR * 8>; // data input type
using dout_t = feature_t;           // data output type

void fc1(
    hls::stream<din_t>& in_stream,
    hls::stream<weight_t>& weight_stream,
    hls::stream<bias_t>& bias_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace fc1_space

#endif