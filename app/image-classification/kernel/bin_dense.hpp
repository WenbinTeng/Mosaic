#ifndef __BIN_DENSE_H__
#define __BIN_DENSE_H__

#include <cmath>

#include "typedef.h"

#include "hls_stream.h"

namespace bin_dense_space {

constexpr int WORD_LEN = 32;   // word length
constexpr int IN_SIZE = 128;   // input size
constexpr int OUT_CLASS = 10;  // output class

using word_t = ap_int<WORD_LEN>;  // word type
using acc_t = ap_int<32>;         // accumulate type
using din_t = ap_int<WORD_LEN>;   // data input type
using dout_t = acc_t;             // data output type

void bin_dense(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
);

}  // namespace bin_dense_space

#endif