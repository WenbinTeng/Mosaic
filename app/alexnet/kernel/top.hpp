#ifndef __TOP_H__
#define __TOP_H__

#include "config.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "conv1.hpp"
#include "pool1.hpp"
#include "conv2.hpp"
#include "pool2.hpp"
#include "conv3.hpp"
#include "conv4.hpp"
#include "conv5.hpp"
#include "pool3.hpp"
#include "fc1.hpp"
#include "fc2.hpp"
#include "fc3.hpp"
#include "top_in.hpp"
#include "top_out.hpp"

void top(
    feature_t img[top_in_space::IMG_H][top_in_space::IMG_W],
    feature_t res[top_out_space::OUT_NUM],
    weight_t fc1_weight[top_in_space::FC1_SIZE][top_in_space::CONV_SIZE],
    weight_t fc2_weight[top_in_space::FC2_SIZE][top_in_space::FC1_SIZE],
    weight_t fc3_weight[top_in_space::FC3_SIZE][top_in_space::FC2_SIZE],
    bias_t fc1_bias[top_in_space::FC1_SIZE],
    bias_t fc2_bias[top_in_space::FC2_SIZE],
    bias_t fc3_bias[top_in_space::FC3_SIZE]
);

#endif