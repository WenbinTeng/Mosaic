#include "top.hpp"

int spam_fil_kernel_0(VectorDataType data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
                      VectorLabelType label[NUM_TRAINING / L_VECTOR_SIZE],
                      VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE]) {

#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=NUM_FEATURES*NUM_TRAINING/D_VECTOR_SIZE port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_TRAINING/L_VECTOR_SIZE port=label offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_FEATURES/F_VECTOR_SIZE port=theta offset=slave bundle=data

    spam_fil(data, label, theta);
    return 0;
}