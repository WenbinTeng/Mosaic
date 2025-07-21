#ifndef __TOP_H__
#define __TOP_H__

#include "typedef.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "top_in.hpp"
#include "fp_conv.hpp"
#include "bin_conv.hpp"
#include "bin_dense.hpp"
#include "top_out.hpp"

void top(
    top_in_space::feature_t img[top_in_space::IMG_CH][top_in_space::IMG_H][top_in_space::IMG_W],
    top_out_space::feature_t res[top_out_space::OUT_CLASS]
);

#endif