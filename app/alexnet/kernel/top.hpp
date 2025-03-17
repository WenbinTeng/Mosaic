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
#include "conv4.hpp"
#include "conv5.hpp"
#include "maxp5.hpp"
#include "full6.hpp"
#include "full7.hpp"
#include "full8.hpp"

void top(
    DTYPE input[3][224][224],
    DTYPE conv1_weight[96][3][11][11],
    DTYPE conv1_bias[96],
    DTYPE conv2_weight[256][96][5][5],
    DTYPE conv2_bias[256],
    DTYPE conv3_weight[384][256][3][3],
    DTYPE conv3_bias[384],
    DTYPE conv4_weight[384][384][3][3],
    DTYPE conv4_bias[384],
    DTYPE conv5_weight[256][384][3][3],
    DTYPE conv5_bias[256],
    DTYPE full6_weight[4096][9216],
    DTYPE full6_bias[4096],
    DTYPE full7_weight[4096][4096],
    DTYPE full7_bias[4096],
    DTYPE full8_weight[1000][4096],
    DTYPE full8_bias[1000],
    DTYPE output[1000]
);

#endif