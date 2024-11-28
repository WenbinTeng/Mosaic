#ifndef __STAGE1_H__
#define __STAGE1_H__

#include <inttypes.h>
#include "hls_stream.h"

#include "../config.h"

#define TILE_SIZE1 128

void stage1(int8_t *in,
            int8_t *query_out, int8_t *key_out, int8_t *value_out,
            int8_t *query_weight, int32_t *query_bias,
            int8_t *key_weight, int32_t *key_bias,
            int8_t *value_weight, int32_t *value_bias,
            float M_query, float M_key, float M_value);

#endif