#ifndef __MAXP2_H__
#define __MAXP2_H__

#include "config.h"

#include "hls_stream.h"

void maxp2(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
);

#endif