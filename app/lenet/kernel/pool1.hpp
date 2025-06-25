#ifndef __POOL1_H__
#define __POOL1_H__

#include "config.h"

#include "hls_stream.h"

void pool1(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream
);

#endif