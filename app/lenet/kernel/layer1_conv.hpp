#ifndef __LAYER1_CONV_H__
#define __LAYER1_CONV_H__

#include "config.h"

#include "hls_stream.h"

void layer1_conv(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
);

#endif