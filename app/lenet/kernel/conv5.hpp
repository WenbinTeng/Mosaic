#ifndef __CONV5_H__
#define __CONV5_H__

#include "hls_stream.h"

void conv5(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
);

#endif