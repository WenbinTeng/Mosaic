#include "top.hpp"

void top(
    vec_data_t data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
    vec_label_t label[NUM_TRAINING / L_VECTOR_SIZE],
    vec_feature_t theta[NUM_FEATURES / F_VECTOR_SIZE]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=label offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=theta offset=slave bundle=data
    
    hls_thread_local hls::stream<data_t> data_stream("data_stream");
    hls_thread_local hls::stream<label_t> label_stream("label_stream");
    hls_thread_local hls::stream<feature_t> theta_stream("theta_stream");

    hls_thread_local hls::task t1(top_in_space::top_in, data, label, data_stream, label_stream);
    hls_thread_local hls::task t2(sgd_space::sgd, data_stream, label_stream, theta_stream);
    hls_thread_local hls::task t3(top_out_space::top_out, theta_stream, theta);
}