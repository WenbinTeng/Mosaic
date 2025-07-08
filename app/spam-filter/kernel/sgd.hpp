#ifndef __SGD_H__
#define __SGD_H__

#include "typedef.h"
#include "lut.h"

#include "hls_stream.h"

namespace sgd_space {

void sgd(
    vec_data_t data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
    vec_label_t label[NUM_TRAINING / L_VECTOR_SIZE],
    vec_feature_t theta[NUM_FEATURES / F_VECTOR_SIZE]
);

}  // namespace sgd_space

#endif