#ifndef __POOL2_H__
#define __POOL2_H__

#include "config.h"

#include "hls_stream.h"

constexpr int CH        = 16;       // channel
constexpr int IN_H      = 10;       // input height
constexpr int IN_W      = 10;       // input width
constexpr int K         = 2;        // kernel size
constexpr int OUT_H     = IN_H / K; // 5, output height
constexpr int OUT_W     = IN_W / K; // 5, output width

using din_t = ap_int<CH * 8>;
using dout_t = ap_int<CH * 8>;

void pool2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

inline void _unpack_input(din_t& input, feature_t _input[CH]) {
#pragma HLS INLINE
    for (int c = 0; c < CH; c++) {
#pragma HLS UNROLL
        _input[c] = input.range(c * 8 + 7, c * 8);
    }
}

inline void _pack_output(feature_t _output[CH], dout_t& output) {
#pragma HLS INLINE
    for (int c = 0; c < CH; c++) {
#pragma HLS UNROLL
        output.range(c * 8 + 7, c * 8) = _output[c];
    }
}

#endif