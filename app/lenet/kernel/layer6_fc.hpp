#ifndef __LAYER6_FC_H__
#define __LAYER6_FC_H__

#include "config.hpp"

#include "hls_stream.h"

void layer6_fc(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
);

#endif