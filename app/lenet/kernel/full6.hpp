#ifndef __FC6_H__
#define __FC6_H__

#include "hls_stream.h"

void full6(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
);

#endif