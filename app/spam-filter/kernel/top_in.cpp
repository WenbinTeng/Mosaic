#include "top_in.hpp"

namespace top_in_space {

void top_in(
    vec_data_t data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
    vec_label_t label[NUM_TRAINING / L_VECTOR_SIZE],
    hls::stream<data_t> &data_stream,
    hls::stream<label_t> &label_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=label offset=slave bundle=data
#pragma HLS INTERFACE axis port=data_stream
#pragma HLS INTERFACE axis port=label_stream

    for (int i = 0; i < NUM_TRAINING; i++) {
        for (int j = 0; j < NUM_FEATURES; j += D_VECTOR_SIZE) {
            vec_data_t tmp_data = data[i * j / D_VECTOR_SIZE];
            for (int w = 0; w < D_VECTOR_SIZE; w++) {
#pragma HLS PIPELINE
                data_stream.write(tmp_data.range((w + 1) * DTYPE_TWIDTH - 1, w * DTYPE_TWIDTH));
            }
        }
        vec_label_t tmp_label = label[i / L_VECTOR_SIZE];
        for (int w = 0; w < D_VECTOR_SIZE; w++) {
#pragma HLS PIPELINE
            label_stream.write(tmp_label.range((w + 1) * LTYPE_WIDTH - 1, w * LTYPE_WIDTH));
        }
    }
}

}  // namespace top_in_space