#ifndef __POOL1_H__
#define __POOL1_H__

#include "config.h"

#include "hls_stream.h"

namespace pool1_space {

constexpr int CH = 6;            // channel
constexpr int IN_H = 28;         // input height
constexpr int IN_W = 28;         // input width
constexpr int K = 2;             // kernel size
constexpr int OUT_H = IN_H / K;  // 14, output height
constexpr int OUT_W = IN_W / K;  // 14, output width

using din_t = ap_int<CH * 8>;    // data input type
using dout_t = ap_int<CH * 8>;   // data output type

void pool1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

}  // namespace pool1_space

#endif