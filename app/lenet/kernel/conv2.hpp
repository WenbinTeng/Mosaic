#ifndef __CONV2_H__
#define __CONV2_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

constexpr int IN_CH     = 6;            // input channel
constexpr int IN_H      = 14;           // input height
constexpr int IN_W      = 14;           // input width
constexpr int K         = 5;            // kernel size
constexpr int OUT_CH    = 16;           // output channel
constexpr int OUT_H     = IN_H - K + 1; // 10, output height
constexpr int OUT_W     = IN_W - K + 1; // 10, output width
constexpr int PAR       = 16;            // parallel factor

using din_t = ap_int<IN_CH * 8>;
using dout_t = ap_int<PAR * 8>;

void conv2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

inline void _unpack_input(din_t& input, feature_t _input[IN_CH]) {
#pragma HLS INLINE
    for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
        _input[ic] = input.range(ic * 8 + 7, ic * 8);
    }
}

inline void _init_weight(weight_t weight[OUT_CH][IN_CH][K][K]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        for (int ic = 0; ic < IN_CH; ic++) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j++) {
                    weight[oc][ic][i][j] = 256 * std::sin(oc * ic * i * j);
                }
            }
        }
    }
}

inline void _init_bias(acc_t bias[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        bias[oc] = 65536 * std::sin(oc);
    }
}

inline void _pack_output(acc_t _output[PAR], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = _output[p];
    }
}

#endif