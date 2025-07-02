#ifndef __FC3_H__
#define __FC3_H__

#include "config.h"

#include "hls_stream.h"

constexpr int PREV_PAR  = 6;    // previous parallel factor
constexpr int IN_SIZE   = 84;   // input size
constexpr int OUT_SIZE  = 10;   // output size
constexpr int PAR       = 10;   // parallel factor

using din_t = ap_int<PREV_PAR * 8>;
using dout_t = ap_int<PAR * 8>;

void fc3(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream,
    const weight_t weight[OUT_SIZE][IN_SIZE],
    const acc_t bias[OUT_SIZE]
);

inline void _unpack_input(din_t& input, feature_t _input[PREV_PAR]) {
#pragma HLS INLINE
    for (int i = 0; i < PREV_PAR; i++) {
        _input[i] = input.range(i * 8 + 7, i * 8);
    }
}

inline void _pack_output(acc_t _output[PAR], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = (feature_t)_output[p];
    }
}

#endif