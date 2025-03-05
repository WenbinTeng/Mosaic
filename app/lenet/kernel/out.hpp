#ifndef __OUT_H__
#define __OUT_H__

#include "hls_stream.h"

void out(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
);

#endif