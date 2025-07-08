#include "top.hpp"

void top(
    vec_data_t data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
    vec_label_t label[NUM_TRAINING / L_VECTOR_SIZE],
    vec_feature_t theta[NUM_FEATURES / F_VECTOR_SIZE]
) {
    sgd_space::sgd(data, label, theta);
}