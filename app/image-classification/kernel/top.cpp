#include "top.hpp"

void top(
    top_in_space::feature_t img[top_in_space::IMG_CH][top_in_space::IMG_H][top_in_space::IMG_W],
    top_out_space::feature_t res[top_out_space::OUT_CLASS]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=img offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=data

    hls_thread_local hls::stream<top_in_space::feature_t> in_stream("in_stream");
    hls_thread_local hls::stream<fp_conv_space::feature_t> fp_conv_out_stream("fp_conv_out_stream");
    hls_thread_local hls::stream<bin_conv_space::feature_t> bin_conv_out_stream1("bin_conv_out_stream1");
    hls_thread_local hls::stream<bin_conv_space::feature_t> bin_conv_out_stream2("bin_conv_out_stream2");
    hls_thread_local hls::stream<bin_dense_space::feature_t> bin_dense_out_stream("bin_dense_out_stream");

    hls_thread_local hls::task t1(top_in_space::top_in, img, in_stream);
    hls_thread_local hls::task t2(fp_conv_space::fp_conv, in_stream, fp_conv_out_stream);
    hls_thread_local hls::task t3(bin_conv_space::bin_conv, fp_conv_out_stream, bin_conv_out_stream1);
    hls_thread_local hls::task t4(bin_conv_space::bin_conv, bin_conv_out_stream1, bin_conv_out_stream2);
    hls_thread_local hls::task t5(bin_dense_space::bin_dense, bin_conv_out_stream2, bin_dense_out_stream);
    hls_thread_local hls::task t6(top_out_space::top_out, bin_dense_out_stream, res);
}