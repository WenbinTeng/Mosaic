#ifndef __POOL2_H__
#define __POOL2_H__

#include "config.h"

#include "hls_stream.h"

void pool2(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream
);

#endif