#ifndef __TOP_H__
#define __TOP_H__

#include "face_detect.h"

int face_detect_kernel_0(unsigned char data[IMAGE_WIDTH],
                 int result_x[RESULT_SIZE],
                 int result_y[RESULT_SIZE],
                 int result_w[RESULT_SIZE],
                 int result_h[RESULT_SIZE],
                 int *result_size);

#endif