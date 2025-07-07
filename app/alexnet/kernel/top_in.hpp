#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "config.h"

#include "hls_stream.h"

namespace top_in_space {

constexpr int IMG_CH = 3;       // image channel
constexpr int IMG_H = 227;      // image height
constexpr int IMG_W = 227;      // image width
constexpr int CONV_SIZE = 9216; // fc1 layer input
constexpr int FC1_SIZE = 4096;  // fc2 layer input
constexpr int FC2_SIZE = 4096;  // fc3 layer input
constexpr int FC3_SIZE = 1000;  // fc3 layer input

void top_in(
    feature_t                   img[IMG_CH][IMG_H][IMG_W],
    weight_t                    fc1_weight[FC1_SIZE][CONV_SIZE],
    weight_t                    fc2_weight[FC2_SIZE][FC1_SIZE],
    weight_t                    fc3_weight[FC3_SIZE][FC2_SIZE],
    bias_t                      fc1_bias[FC1_SIZE],
    bias_t                      fc2_bias[FC2_SIZE],
    bias_t                      fc3_bias[FC3_SIZE],
    hls::stream<feature_t>&     img_stream,
    hls::stream<weight_t>&      fc1_weight_stream,
    hls::stream<bias_t>&        fc1_bias_stream,
    hls::stream<weight_t>&      fc2_weight_stream,
    hls::stream<bias_t>&        fc2_bias_stream,
    hls::stream<weight_t>&      fc3_weight_stream,
    hls::stream<bias_t>&        fc3_bias_stream
);

} // namespace top_in_space

#endif