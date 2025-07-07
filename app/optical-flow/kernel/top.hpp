#ifndef __TOP_H__
#define __TOP_H__

#include "typedef.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "grad_xy_calc.hpp"
#include "grad_z_calc.hpp"
#include "grad_wgt_y.hpp"
#include "grad_wgt_x.hpp"
#include "outer_product.hpp"
#include "ten_wgt_y.hpp"
#include "ten_wgt_x.hpp"
#include "flow_calc.hpp"
#include "top_in.hpp"
#include "top_out.hpp"

void top(
    frames_t frames[MAX_HEIGHT][MAX_WIDTH],
    velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]
);

#endif