#ifndef __LAYER4_FC_H__
#define __LAYER4_FC_H__

#include "config.h"

#include "hls_stream.h"

void layer4_fc(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
);

#endif