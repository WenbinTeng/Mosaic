#include "top.hpp"

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
                     volatile DTYPE output[1000]) {

    DTYPE c1_output[96][55][55];
    DTYPE s1_output[96][31][31];
    DTYPE c2_output[256][27][27];
    DTYPE s2_output[256][15][15];
    DTYPE c3_output[384][15][15];
    DTYPE c4_output[384][15][15];
    DTYPE c5_output[256][13][13];
    DTYPE s3_output[256][6][6];
    DTYPE f1_output[4096];
    DTYPE f2_output[4096];

    layer_c1(input, c1_weights, c1_bias, c1_output);
    layer_s1(c1_output, s1_output);
    layer_c2(s1_output, c2_weights, c2_bias, c2_output);
    layer_s2(c2_output, s2_output);
    layer_c3(s2_output, c3_weights, c3_bias, c3_output);
    layer_c4(c3_output, c4_weights, c4_bias, c4_output);
    layer_c5(c4_output, c5_weights, c5_bias, c5_output);
    layer_s3(c5_output, s3_output);
    layer_f1(s3_output, f1_weights, f1_bias, f1_output);
    layer_f2(f1_output, f2_weights, f2_bias, f2_output);
    layer_f3(f2_output, f3_weights, f3_bias, output);

    return 0;
}