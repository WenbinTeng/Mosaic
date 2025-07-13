#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<feature_t> &theta_stream,
    vec_feature_t theta[NUM_FEATURES / F_VECTOR_SIZE]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=theta_stream
#pragma HLS INTERFACE m_axi port=theta offset=slave bundle=data

    for (int i = 0; i < NUM_FEATURES; i += F_VECTOR_SIZE) {
        vec_feature_t vec_theta = 0;
        for (int w = 0; w < F_VECTOR_SIZE; w++) {
#pragma HLS PIPELINE
            feature_t theta = theta_stream.read();
            vec_theta.range((w + 1) * FTYPE_TWIDTH - 1, w * FTYPE_TWIDTH) = theta;
        }
        theta[i / F_VECTOR_SIZE] = vec_theta;
    }
}

}  // namespace top_out_space