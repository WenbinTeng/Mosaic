#include "top.hpp"

int rendering_kernel_0(bit32 input[3 * NUM_3D_TRI], bit32 output[NUM_FB]) {

#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=3*NUM_3D_TRI port=input offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_FB port=output offset=slave bundle=data

    rendering(input, output);
    return 0;
}