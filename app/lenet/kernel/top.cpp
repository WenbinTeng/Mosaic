#include "top.hpp"

void top(
    feature_t img[top_in_space::IMG_H][top_in_space::IMG_W],
    feature_t res[top_out_space::OUT_NUM]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=img offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=data

    hls_thread_local hls::stream<feature_t> in_stream("in_stream");
    hls_thread_local hls::stream<conv1_space::dout_t> conv1_out_stream("conv1_out_stream");
    hls_thread_local hls::stream<pool1_space::dout_t> pool1_out_stream("pool1_out_stream");
    hls_thread_local hls::stream<conv2_space::dout_t> conv2_out_stream("conv2_out_stream");
    hls_thread_local hls::stream<pool2_space::dout_t> pool2_out_stream("pool2_out_stream");
    hls_thread_local hls::stream<fc1_space::dout_t> fc1_out_stream("fc1_out_stream");
    hls_thread_local hls::stream<fc2_space::dout_t> fc2_out_stream("fc2_out_stream");
    hls_thread_local hls::stream<fc3_space::dout_t> fc3_out_stream("fc3_out_stream");
    hls_thread_local hls::stream<feature_t> out_stream("out_stream");

    hls_thread_local hls::task t1(top_in_space::top_in, img, in_stream);
    hls_thread_local hls::task t2(conv1_space::conv1, in_stream, conv1_out_stream);
    hls_thread_local hls::task t3(pool1_space::pool1, conv1_out_stream, pool1_out_stream);
    hls_thread_local hls::task t4(conv2_space::conv2, pool1_out_stream, conv2_out_stream);
    hls_thread_local hls::task t5(pool2_space::pool2, conv2_out_stream, pool2_out_stream);
    hls_thread_local hls::task t6(fc1_space::fc1, pool2_out_stream, fc1_out_stream);
    hls_thread_local hls::task t7(fc2_space::fc2, fc1_out_stream, fc2_out_stream);
    hls_thread_local hls::task t8(fc3_space::fc3, fc2_out_stream, fc3_out_stream);
    hls_thread_local hls::task t9(top_out_space::top_out, fc3_out_stream, res);
}