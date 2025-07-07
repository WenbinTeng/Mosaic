#ifndef __TOP_H__
#define __TOP_H__

#include "typedef.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "projection.hpp"
#include "rasterization1.hpp"
#include "rasterization2.hpp"
#include "zculling.hpp"
#include "coloring.hpp"
#include "top_in.hpp"
#include "top_out.hpp"

void top(
    bit32 input[3 * NUM_3D_TRI],
    bit32 output[NUM_FB]
);

#endif