#ifndef __STAGE3_H__
#define __STAGE3_H__

#include <inttypes.h>
#include <math.h>
#include <algorithm>
#include "hls_stream.h"

#include "../config.h"

#define TILE_SIZE3 128

void stage3(int8_t *fc_in,
            int8_t *dense_weight,
            int32_t *dense_bias,
            int8_t *dense_out,
            float M_gelu, float M_stage3);

#endif
