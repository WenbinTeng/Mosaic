#ifndef __FC3_H__
#define __FC3_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

namespace fc3_space {

constexpr int PREV_PAR = 6;          // previous parallel factor
constexpr int IN_SIZE = 84;          // input size
constexpr int OUT_SIZE = 10;         // output size
constexpr int PAR = 1;               // parallel factor

using din_t = ap_int<PREV_PAR * 8>;  // data input type
using dout_t = ap_int<PAR * 8>;      // data output type

using namespace fc3_space;

void fc3(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

}  // namespace fc3_space

#endif