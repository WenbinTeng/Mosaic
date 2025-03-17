#ifndef __MAXP1_H__
#define __MAXP1_H__

#include "config.h"

#include "hls_stream.h"

void maxp1(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
);

#endif