#ifndef __TOP_H__
#define __TOP_H__

#include "config.hpp"

#include "hls_stream.h"
#include "hls_task.h"

#include "layer1_conv.hpp"
#include "layer2_maxp.hpp"
#include "layer3_conv.hpp"
#include "layer4_maxp.hpp"
#include "layer5_conv.hpp"
#include "layer6_fc.hpp"
#include "layer7_out.hpp"

void top(
    DTYPE input[32][32],
    DTYPE conv1_weight[6][5][5],
    DTYPE conv1_bias[6],
    DTYPE conv3_weight[16][6][5][5],
    DTYPE conv3_bias[16],
    DTYPE conv5_weight[120][16][5][5],
    DTYPE conv5_bias[120],
    DTYPE full6_weight[84][120],
    DTYPE full6_bias[84],
    DTYPE out_weight[10][84],
    DTYPE out_bias[10],
    DTYPE output[10]
);

#endif