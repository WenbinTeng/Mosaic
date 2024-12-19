#ifndef __TOP_H__
#define __TOP_H__

#include "optical_flow.hpp"

int optical_flow_kernel_0(frames_t frames[MAX_HEIGHT][MAX_WIDTH],
                          velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]);

#endif