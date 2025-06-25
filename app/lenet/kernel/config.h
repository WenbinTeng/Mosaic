#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "ap_int.h"

using feature_t = ap_int<8>;
using weight_t  = ap_int<8>;
using acc_t     = ap_int<16>; 

inline acc_t relu(acc_t x) {
#pragma HLS INLINE
    return (x > 0) ? x : 0;
}

#endif