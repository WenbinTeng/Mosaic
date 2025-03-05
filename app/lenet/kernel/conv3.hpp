#ifndef __CONV2_H__
#define __CONV2_H__

#include "hls_stream.h"

void conv3(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
);

#endif