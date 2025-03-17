#ifndef __CONV1_H__
#define __CONV1_H__

#include "config.h"

#include "hls_stream.h"

void conv1(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
);

#endif