#ifndef __FULL4_H__
#define __FULL4_H__

#include "config.h"

#include "hls_stream.h"

void full4(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
);

#endif