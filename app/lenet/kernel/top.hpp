#ifndef __TOP_H__
#define __TOP_H__

#include "config.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "conv1.hpp"
#include "pool1.hpp"
#include "conv2.hpp"
#include "pool2.hpp"
#include "fc1.hpp"
#include "fc2.hpp"
#include "fc3.hpp"
#include "top_in.hpp"
#include "top_out.hpp"

void top(
    feature_t img[top_in_space::IMG_H][top_in_space::IMG_W],
    feature_t res[top_out_space::OUT_NUM]
);

#endif