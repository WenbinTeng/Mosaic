#ifndef __CONV1_H__
#define __CONV1_H__

#include <cmath>

#include "config.h"

#include "hls_stream.h"

constexpr int IN_CH     = 1;                // input channel
constexpr int IN_H      = 32;               // input height
constexpr int IN_W      = 32;               // input width
constexpr int K         = 5;                // kernel size
constexpr int OUT_CH    = 6;                // output channel
constexpr int OUT_H     = IN_H - K + 1;     // 28, output height
constexpr int OUT_W     = IN_W - K + 1;     // 28, output width

using din_t = ap_int<IN_CH * 8>;
using dout_t = ap_int<OUT_CH * 8>;

void conv1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
);

inline void _unpack_input(din_t& input, feature_t& _input) {
#pragma HLS INLINE
    _input = input;
}

inline void _init_weight(weight_t weight[OUT_CH][K][K]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < K; j++) {
                weight[oc][i][j] = 256 * std::sin(oc * i * j);
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

inline void _pack_output(acc_t _output[OUT_CH], dout_t& output) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
#pragma HLS UNROLL
        output.range(oc * 8 + 7, oc * 8) = (feature_t)_output[oc];
    }
}

#endif