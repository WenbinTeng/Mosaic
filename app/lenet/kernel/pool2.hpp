#ifndef __POOL2_H__
#define __POOL2_H__

#include "config.h"

#include "hls_stream.h"

namespace pool2_space {

constexpr int CH = 16;          // channel
constexpr int IN_H = 10;        // input height
constexpr int IN_W = 10;        // input width
constexpr int K = 2;            // kernel size
constexpr int OUT_H = IN_H / K; // 5, output height
constexpr int OUT_W = IN_W / K; // 5, output width

using din_t = ap_int<CH * 8>;  // data input type
using dout_t = ap_int<CH * 8>; // data output type

void pool2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

}  // namespace pool2_space

#endif