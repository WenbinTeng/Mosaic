#ifndef __LENET_H__
#define __LENET_H__

#include "typedef.h"

void layer_c1(volatile DTYPE input[32][32],
              volatile DTYPE weights[6][5][5],
              volatile DTYPE bias[6],
              volatile DTYPE output[6][28][28]);

void layer_s2(volatile DTYPE input[6][28][28],
              volatile DTYPE output[6][14][14]);

void layer_c3(volatile DTYPE input[6][14][14],
              volatile DTYPE weights[16][6][5][5],
              volatile DTYPE bias[16],
              volatile DTYPE output[16][10][10]);

void layer_s4(volatile DTYPE input[16][10][10],
              volatile DTYPE output[16][5][5]);

void layer_c5(volatile DTYPE input[16][5][5],
              volatile DTYPE weights[120][16][5][5],
              volatile DTYPE bias[120],
              volatile DTYPE output[120]);

void layer_f6(volatile DTYPE input[120],
              volatile DTYPE weights[84][120],
              volatile DTYPE bias[84],
              volatile DTYPE output[84]);

void layer_output(volatile DTYPE input[84],
                  volatile DTYPE weights[10][84],
                  volatile DTYPE bias[10],
                  volatile DTYPE output[10]);

#endif
