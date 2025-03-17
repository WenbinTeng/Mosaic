#ifndef __TOP_H__
#define __TOP_H__

#include "config.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "conv1.hpp"
#include "maxp1.hpp"
#include "conv2.hpp"
#include "maxp2.hpp"
#include "conv3.hpp"
#include "full4.hpp"
#include "full5.hpp"

void top(
    DTYPE input[32][32],
    DTYPE layer1_conv_weight[6][5][5],
    DTYPE layer1_conv_bias[6],
    DTYPE layer2_conv_weight[16][6][5][5],
    DTYPE layer2_conv_bias[16],
    DTYPE layer3_conv_weight[120][16][5][5],
    DTYPE layer3_conv_bias[120],
    DTYPE layer4_full_weight[84][120],
    DTYPE layer4_full_bias[84],
    DTYPE layer5_full_weight[10][84],
    DTYPE layer5_full_bias[10],
    DTYPE output[10]
);

#endif