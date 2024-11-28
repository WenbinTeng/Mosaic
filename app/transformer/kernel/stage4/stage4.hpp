#ifndef __STAGE4_H__
#define __STAGE4_H__

#include <inttypes.h>
#include <math.h>
#include "hls_stream.h"

#include "../config.h"

#define TILE_SIZE4 128

void stage4(int8_t *fc_in,
            int8_t *skip,
            int8_t *dense_weight, int32_t *dense_bias, int8_t *dense_out,
            int16_t *norm_weight, int16_t *norm_bias,
            float M_dense_acc, float M_residual,float M_stage4);

#endif
