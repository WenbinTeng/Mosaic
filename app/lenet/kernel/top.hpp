#ifndef __TOP_H__
#define __TOP_H__

#include "lenet.h"

int lenet_kernel_0(volatile DTYPE input[32][32],
                   volatile DTYPE c1_weights[6][5][5],
                   volatile DTYPE c1_bias[6],
                   volatile DTYPE c3_weights[16][6][5][5],
                   volatile DTYPE c3_bias[16],
                   volatile DTYPE c5_weights[120][16][5][5],
                   volatile DTYPE c5_bias[120],
                   volatile DTYPE f6_weights[84][120],
                   volatile DTYPE f6_bias[84],
                   volatile DTYPE output_weights[10][84],
                   volatile DTYPE output_bias[10],
                   volatile DTYPE output[10]);

#endif