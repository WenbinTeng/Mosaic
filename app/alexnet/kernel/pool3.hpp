#ifndef __POOL3_H__
#define __POOL3_H__

#include "config.h"

#include "hls_stream.h"

namespace pool3_space {

constexpr int CH = 256;         // channel
constexpr int IN_H = 13;        // input height
constexpr int IN_W = 13;        // input width
constexpr int K = 3;            // kernel size
constexpr int OUT_H = IN_H / 2; // 6, output height
constexpr int OUT_W = IN_W / 2; // 6, output width
constexpr int PAR = 16;         // parallel factor

using din_t = ap_int<PAR * 8>;  // data input type
using dout_t = ap_int<PAR * 8>; // data output type

void pool3(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

}  // namespace pool3_space

#endif