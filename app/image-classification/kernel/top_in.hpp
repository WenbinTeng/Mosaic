#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_in_space {

constexpr int IMG_CH = 3;
constexpr int IMG_H = 32;
constexpr int IMG_W = 32;

using feature_t = ap_fixed<16, 4>;

void top_in(
    feature_t img[IMG_CH][IMG_H][IMG_W],
    hls::stream<feature_t> &img_stream
);

}  // namespace top_in_space

#endif