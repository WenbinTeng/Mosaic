#ifndef __SAMP4_H__
#define __SAMP4_H__

#include "hls_stream.h"

void samp4(
    hls::stream<float>& in_stream,
    hls::stream<float>& out_stream
);

#endif