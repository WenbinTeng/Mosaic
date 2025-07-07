#ifndef __POOL1_H__
#define __POOL1_H__

#include "config.h"

#include "hls_stream.h"

namespace pool1_space {

constexpr int CH = 96;          // channel
constexpr int IN_H = 55;        // input height
constexpr int IN_W = 55;        // input width
constexpr int K = 3;            // kernel size
constexpr int OUT_H = IN_H / 2; // 27, output height
constexpr int OUT_W = IN_W / 2; // 27, output width
constexpr int PAR = 4;          // parallel factor

using din_t = ap_int<PAR * 8>;  // data input type
using dout_t = ap_int<PAR * 8>; // data output type

void pool1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

}  // namespace pool1_space

#endif