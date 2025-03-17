#ifndef __LAYER1_MAXP_H__
#define __LAYER1_MAXP_H__

#include "config.h"

#include "hls_stream.h"

void layer1_maxp(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
);

#endif