#include "top_in.hpp"

namespace top_in_space {

void top_in(
    whole_digit_t global_training_set[NUM_TRAINING],
    whole_digit_t global_test_set[NUM_TEST],
    hls::stream<whole_digit_t> &training_stream,
    hls::stream<whole_digit_t> &test_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=global_training_set offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=global_test_set offset=slave bundle=data
#pragma HLS INTERFACE axis port=training_stream
#pragma HLS INTERFACE axis port=test_stream

    for (int i = 0; i < NUM_TRAINING; i++) {
#pragma HLS PIPELINE
        training_stream.write(global_training_set[i]);
    }
    for (int i = 0; i < NUM_TEST; i++) {
#pragma HLS PIPELINE
        test_stream.write(global_test_set[i]);
    }
}

}  // namespace top_in_space