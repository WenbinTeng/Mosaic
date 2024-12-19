#include "top.hpp"

int optical_flow_kernel_0(frames_t frames[MAX_HEIGHT][MAX_WIDTH],
                          velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]) {

#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=MAX_HEIGHT*MAX_WIDTH port=frames offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=MAX_HEIGHT*MAX_WIDTH port=outputs offset=slave bundle=data

    optical_flow(frames, outputs);
    return 0;
}