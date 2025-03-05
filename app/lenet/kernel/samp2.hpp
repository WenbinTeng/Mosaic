#ifndef __SAMP2_H__
#define __SAMP2_H__

#include "hls_stream.h"

void samp2(
    hls::stream<float>& in_stream,
    hls::stream<float>& out_stream
);

#endif