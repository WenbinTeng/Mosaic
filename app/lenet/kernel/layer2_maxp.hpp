#ifndef __LAYER2_MAXP_H__
#define __LAYER2_MAXP_H__

#include "config.h"

#include "hls_stream.h"

void layer2_maxp(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
);

#endif