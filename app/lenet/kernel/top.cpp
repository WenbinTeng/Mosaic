#include "top.hpp"

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
                   volatile DTYPE output[10]) {

#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=1024 port=input offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=150 port=c1_weights offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=6 port=c1_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=2400 port=c3_weights offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=16 port=c3_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=48000 port=c5_weights offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=120 port=c5_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=10080 port=f6_weights offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=84 port=f6_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=840 port=output_weights offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=10 port=output_bias offset=slave bundle=data

    DTYPE c1_output[6][28][28];
    DTYPE s2_output[6][14][14];
    DTYPE c3_output[16][10][10];
    DTYPE s4_output[16][5][5];
    DTYPE c5_output[120];
    DTYPE f6_output[84];

    layer_c1(input, c1_weights, c1_bias, c1_output);
    layer_s2(c1_output, s2_output);
    layer_c3(s2_output, c3_weights, c3_bias, c3_output);
    layer_s4(c3_output, s4_output);
    layer_c5(s4_output, c5_weights, c5_bias, c5_output);
    layer_f6(c5_output, f6_weights, f6_bias, f6_output);
    layer_output(f6_output, output_weights, output_bias, output);

    return 0;
}