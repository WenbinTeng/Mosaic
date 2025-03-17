#ifndef __MAXP5_H__
#define __MAXP5_H__

#include "config.h"

#include "hls_stream.h"

void maxp5(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& out_stream
);

#endif