#ifndef __TOP_OUT_H__
#define __TOP_OUT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_out_space {

void top_out(
    hls::stream<feature_t> &theta_stream,
    vec_feature_t theta[NUM_FEATURES / F_VECTOR_SIZE]
);

}  // namespace top_out_space

#endif