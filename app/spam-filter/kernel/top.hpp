#ifndef __TOP_H__
#define __TOP_H__

#include "sgd.hpp"

void spam_fil_kernel_0(VectorDataType data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
                       VectorLabelType label[NUM_TRAINING / L_VECTOR_SIZE],
                       VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE],
                       bool readLabels, bool writeOutput);

#endif