#include "../kernel/top.hpp"

#include "xil_printf.h"
#include "xiltimer.h"
#include "xparameters.h"

#include "hls_stream.h"

#include "stdlib.h"

int main(int argc, char const* argv[]) {
    float input[32][32];
    float conv1_weight[6][5][5];
    float conv1_bias[6];
    float conv3_weight[16][6][5][5];
    float conv3_bias[16];
    float conv5_weight[120][16][5][5];
    float conv5_bias[120];
    float full6_weight[84][120];
    float full6_bias[84];
    float out_weight[10][84];
    float out_bias[10];
    float output[10];
    
    top(input, conv1_weight, conv1_bias, conv3_weight, conv3_bias, conv5_weight, conv5_bias, full6_weight, full6_bias, out_weight, out_bias, output);

    xprintf("type\tprob");
    for (int n = 0; n < 10; n++) {
        xprintf("%d\t%d", n, output[n]);
    }

    return 0;
}
