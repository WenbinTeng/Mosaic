#ifndef __FC1_H__
#define __FC1_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace fc1_space {

constexpr int PREV_PAR = 16;        // previous parallel factor
constexpr int IN_SIZE = 16 * 5 * 5; // 400, input size
constexpr int OUT_SIZE = 120;       // output size
constexpr int PAR = 3;              // parallel factor

using din_t = ap_int<PREV_PAR * 8>; // data input type
using dout_t = ap_int<PAR * 8>;     // data output type

using namespace fc1_space;

void fc1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

} // namespace fc1_space

#endif