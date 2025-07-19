#ifndef __TOP_H__
#define __TOP_H__

#include "typedef.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "top_in.hpp"
#include "scale.hpp"
#include "vj.hpp"
#include "top_out.hpp"

void top(
    unsigned char data[IMAGE_WIDTH],
    int result_x[RESULT_SIZE],
    int result_y[RESULT_SIZE],
    int result_w[RESULT_SIZE],
    int result_h[RESULT_SIZE],
    int *result_size
);

#endif