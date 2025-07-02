#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "config.h"

#include "hls_stream.h"

constexpr int IMG_H     = 32; // image height
constexpr int IMG_W     = 32; // image width

void top_in(
    feature_t img[IMG_H][IMG_W],
    hls::stream<feature_t>& img_stream
);

#endif