#ifndef __LAYER4_MAXP_H__
#define __LAYER4_MAXP_H__

#include "config.hpp"

#include "hls_stream.h"

void layer4_maxp(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
);

#endif