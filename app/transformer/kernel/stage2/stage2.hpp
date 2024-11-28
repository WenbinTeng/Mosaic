#ifndef __STAGE2_H__
#define __STAGE2_H__

#include <inttypes.h>
#include <math.h>
#include "hls_stream.h"

#include "../config.h"

#define TILE_SIZE2 128

void stage2(int8_t *query_in, int8_t *key_in, int8_t *value_in, int8_t *skip_in,
            int8_t *dense_weight_t, int32_t *dense_bias,
            float M_attn_probs, float M_attn_out, float M_dense_out, float M_residual,
            int16_t *norm_weight, int16_t *norm_bias, float M_stage2,
            int8_t *stage2_out);

#endif