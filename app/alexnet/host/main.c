#include "../kernel/top.hpp"

#include "xil_printf.h"
#include "xiltimer.h"
#include "xparameters.h"

#include "stdlib.h"

int main(int argc, char const *argv[])
{
    float input[3][224][224];
    float conv1_weight[96][3][11][11];
    float conv1_bias[96];
    float conv2_weight[256][96][5][5];
    float conv2_bias[256];
    float conv3_weight[384][256][3][3];
    float conv3_bias[384];
    float conv4_weight[384][384][3][3];
    float conv4_bias[384];
    float conv5_weight[256][384][3][3];
    float conv5_bias[256];
    float full6_weight[4096][9216];
    float full6_bias[4096];
    float full7_weight[4096][4096];
    float full7_bias[4096];
    float full8_weight[1000][4096];
    float full8_bias[1000];
    float output[1000];
    
    top(input, conv1_weight, conv1_bias, conv2_weight, conv2_bias, conv3_weight, conv3_bias, conv4_weight, conv4_bias, conv5_weight, conv5_bias, full6_weight, full6_bias, full7_weight, full7_bias, full8_weight, full8_bias);

    xprintf("type\tprob\n");
    for (int n = 0; n < 1000; n++) {
        xprintf("%d\t%d\n", n, output[n]);
    }

    return 0;
}
