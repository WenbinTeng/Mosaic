#ifndef __LAYER7_OUT_H__
#define __LAYER7_OUT_H__

#include "config.hpp"

#include "hls_stream.h"

void layer7_out(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
);

#endif