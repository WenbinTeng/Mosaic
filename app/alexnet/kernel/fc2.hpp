#ifndef __FC2_H__
#define __FC2_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace fc2_space {

constexpr int IN_SIZE = 4096;  // input size
constexpr int OUT_SIZE = 4096; // output size
constexpr int PAR = 16;        // parallel factor

using din_t = feature_t;  // data input type
using dout_t = feature_t; // data output type

void fc2(
    hls::stream<din_t>& in_stream,
    hls::stream<weight_t>& weight_stream,
    hls::stream<bias_t>& bias_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace fc2_space

#endif