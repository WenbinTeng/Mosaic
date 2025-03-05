#ifndef __CONV1_H__
#define __CONV1_H__

#include "hls_stream.h"

void conv1(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
);

#endif