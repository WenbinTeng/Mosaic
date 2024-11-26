#ifndef __ALEXNET_H__
#define __ALEXNET_H__

#include "typedef.h"

void layer_c1(volatile DTYPE input[3][227][227],
              volatile DTYPE weights[96][3][11][11],
              volatile DTYPE bias[96],
              volatile DTYPE output[96][55][55]);

void layer_s1(volatile DTYPE input[96][55][55],
              volatile DTYPE output[96][31][31]);

void layer_c2(volatile DTYPE input[96][31][31],
              volatile DTYPE weights[256][96][5][5],
              volatile DTYPE bias[256],
              volatile DTYPE output[256][27][27]);

void layer_s2(volatile DTYPE input[256][27][27],
              volatile DTYPE output[256][15][15]);

void layer_c3(volatile DTYPE input[256][15][15],
              volatile DTYPE weights[384][256][3][3],
              volatile DTYPE bias[384],
              volatile DTYPE output[384][15][15]);

void layer_c4(volatile DTYPE input[384][15][15],
              volatile DTYPE weights[384][384][3][3],
              volatile DTYPE bias[384],
              volatile DTYPE output[384][15][15]);

void layer_c5(volatile DTYPE input[384][15][15],
              volatile DTYPE weights[256][384][3][3],
              volatile DTYPE bias[256],
              volatile DTYPE output[256][13][13]);

void layer_s3(volatile DTYPE input[256][13][13],
              volatile DTYPE output[256][6][6]);

void layer_f1(volatile DTYPE input[256][6][6],
              volatile DTYPE weights[4096][256][6][6],
              volatile DTYPE bias[4096],
              volatile DTYPE output[4096]);

void layer_f2(volatile DTYPE input[4096],
              volatile DTYPE weights[4096][4096],
              volatile DTYPE bias[4096],
              volatile DTYPE output[4096]);

void layer_f3(volatile DTYPE input[4096],
              volatile DTYPE weights[1000][4096],
              volatile DTYPE bias[1000],
              volatile DTYPE output[1000]);

#endif