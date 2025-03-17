#ifndef __TOP_H__
#define __TOP_H__

#include "config.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "layer1_conv.hpp"
#include "layer1_maxp.hpp"
#include "layer2_conv.hpp"
#include "layer2_maxp.hpp"
#include "layer3_conv.hpp"
#include "layer4_fc.hpp"
#include "layer5_fc.hpp"

void top(
    DTYPE input[32][32],
    DTYPE layer1_conv_weight[6][5][5],
    DTYPE layer1_conv_bias[6],
    DTYPE layer2_conv_weight[16][6][5][5],
    DTYPE layer2_conv_bias[16],
    DTYPE layer3_conv_weight[120][16][5][5],
    DTYPE layer3_conv_bias[120],
    DTYPE layer4_fc_weight[84][120],
    DTYPE layer4_fc_bias[84],
    DTYPE layer5_fc_weight[10][84],
    DTYPE layer5_fc_bias[10],
    DTYPE output[10]
);

#endif