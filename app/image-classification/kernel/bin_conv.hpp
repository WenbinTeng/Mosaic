#ifndef __BIN_CONV_H__
#define __BIN_CONV_H__

#include <cmath>

#include "typedef.h"

#include "hls_stream.h"

namespace bin_conv_space {

constexpr int WORD_LEN = 32;                       // word length
constexpr int IN_CH = 128;                         // input channel
constexpr int IN_H = 32;                           // input height
constexpr int IN_W = 32;                           // input width
constexpr int IN_PACKS = IN_CH / WORD_LEN;         // input packages
constexpr int K = 3;                               // kernel size
constexpr int S = 1;                               // stride size
constexpr int P = 1;                               // padding size
constexpr int OUT_CH = 128;                        // output channel
constexpr int OUT_H = (IN_H - K + 2 * P) / S + 1;  // output height
constexpr int OUT_W = (IN_W - K + 2 * P) / S + 1;  // output width
constexpr int OUT_PACKS = OUT_CH / WORD_LEN;       // output packages

using word_t = ap_int<WORD_LEN>;  // word type
using acc_t = ap_int<32>;         // accumulation type
using din_t = ap_int<WORD_LEN>;   // data input type
using dout_t = ap_int<WORD_LEN>;  // data output type

void bin_conv(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
);

}  // namespace bin_conv_space

#endif