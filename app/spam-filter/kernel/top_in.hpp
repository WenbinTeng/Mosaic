#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_in_space {

void top_in(
    vec_data_t data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
    vec_label_t label[NUM_TRAINING / L_VECTOR_SIZE],
    hls::stream<data_t> &data_stream,
    hls::stream<label_t> &label_stream
);

}  // namespace top_in_space

#endif