#ifndef __FC2_H__
#define __FC2_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace fc2_space {

constexpr int PREV_PAR = 3;         // previous parallel factor
constexpr int IN_SIZE = 120;        // input size
constexpr int OUT_SIZE = 84;        // output size
constexpr int PAR = 6;              // parallel factor

using din_t = ap_int<PREV_PAR * 8>; // data input type
using dout_t = ap_int<PAR * 8>;     // data output type

void fc2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace fc2_space

#endif