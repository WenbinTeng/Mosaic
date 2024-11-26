#ifndef __TOP_H__
#define __TOP_H__

#include "alexnet.hpp"

int alexnet_kernel_0(volatile DTYPE input[3][227][227],
                     volatile DTYPE c1_weights[96][3][11][11],
                     volatile DTYPE c1_bias[96],
                     volatile DTYPE c2_weights[256][96][5][5],
                     volatile DTYPE c2_bias[256],
                     volatile DTYPE c3_weights[384][256][3][3],
                     volatile DTYPE c3_bias[384],
                     volatile DTYPE c4_weights[384][384][3][3],
                     volatile DTYPE c4_bias[384],
                     volatile DTYPE c5_weights[256][384][3][3],
                     volatile DTYPE c5_bias[256],
                     volatile DTYPE f1_weights[4096][256][6][6],
                     volatile DTYPE f1_bias[4096],
                     volatile DTYPE f2_weights[4096][4096],
                     volatile DTYPE f2_bias[4096],
                     volatile DTYPE f3_weights[1000][4096],
                     volatile DTYPE f3_bias[1000],
                     volatile DTYPE output[1000]);

#endif