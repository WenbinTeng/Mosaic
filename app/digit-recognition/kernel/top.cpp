#include "top.hpp"

void top(
    whole_digit_t global_training_set[NUM_TRAINING],
    whole_digit_t global_test_set[NUM_TEST],
    label_t global_results[NUM_TEST]
) {
#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=NUM_TRAINING port=global_training_set offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_TEST port=global_test_set offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_TEST port=global_results offset=slave bundle=data

    hls_thread_local hls::stream<whole_digit_t> training_stream("training_stream");
    hls_thread_local hls::stream<whole_digit_t> test_stream("test_stream");
    hls_thread_local hls::stream<label_t> result_stream("result_stream");

    hls_thread_local hls::task t1(top_in_space::top_in, global_training_set, global_test_set, training_stream, test_stream);
    hls_thread_local hls::task t2(knn_space::knn, training_stream, test_stream, result_stream);
    hls_thread_local hls::task t3(top_out_space::top_out, result_stream, global_results);
}