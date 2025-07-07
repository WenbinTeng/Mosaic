#include "top.hpp"

void top(
    feature_t img[top_in_space::IMG_CH][top_in_space::IMG_H][top_in_space::IMG_W],
    feature_t res[top_out_space::OUT_NUM],
    weight_t fc1_weight[top_in_space::FC1_SIZE][top_in_space::CONV_SIZE],
    weight_t fc2_weight[top_in_space::FC2_SIZE][top_in_space::FC1_SIZE],
    weight_t fc3_weight[top_in_space::FC3_SIZE][top_in_space::FC2_SIZE],
    bias_t fc1_bias[top_in_space::FC1_SIZE],
    bias_t fc2_bias[top_in_space::FC2_SIZE],
    bias_t fc3_bias[top_in_space::FC3_SIZE]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=img offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=res offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc1_weight offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc2_weight offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc3_weight offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc1_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc2_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc3_bias offset=slave bundle=data
#pragma HLS DATAFLOW

    hls_thread_local hls::stream<feature_t> img_stream("img_stream");
    hls_thread_local hls::stream<weight_t> fc1_weight_stream("fc1_weight_stream");
    hls_thread_local hls::stream<weight_t> fc2_weight_stream("fc2_weight_stream");
    hls_thread_local hls::stream<weight_t> fc3_weight_stream("fc3_weight_stream");
    hls_thread_local hls::stream<bias_t> fc1_bias_stream("fc1_bias_stream");
    hls_thread_local hls::stream<bias_t> fc2_bias_stream("fc2_bias_stream");
    hls_thread_local hls::stream<bias_t> fc3_bias_stream("fc3_bias_stream");
    hls_thread_local hls::stream<conv1_space::dout_t> conv1_out_stream("conv1_out_stream");
    hls_thread_local hls::stream<pool1_space::dout_t> pool1_out_stream("pool1_out_stream");
    hls_thread_local hls::stream<conv2_space::dout_t> conv2_out_stream("conv2_out_stream");
    hls_thread_local hls::stream<pool2_space::dout_t> pool2_out_stream("pool2_out_stream");
    hls_thread_local hls::stream<conv3_space::dout_t> conv3_out_stream("conv3_out_stream");
    hls_thread_local hls::stream<conv4_space::dout_t> conv4_out_stream("conv4_out_stream");
    hls_thread_local hls::stream<conv5_space::dout_t> conv5_out_stream("conv5_out_stream");
    hls_thread_local hls::stream<pool3_space::dout_t> pool3_out_stream("pool3_out_stream");
    hls_thread_local hls::stream<fc1_space::dout_t> fc1_out_stream("fc1_out_stream");
    hls_thread_local hls::stream<fc2_space::dout_t> fc2_out_stream("fc2_out_stream");
    hls_thread_local hls::stream<fc3_space::dout_t> fc3_out_stream("fc3_out_stream");

    hls_thread_local hls::task t1(top_in_space::top_in, img, fc1_weight, fc2_weight, fc3_weight, fc1_bias, fc2_bias, fc3_bias, img_stream, fc1_weight_stream, fc2_weight_stream, fc3_weight_stream, fc1_bias_stream, fc2_bias_stream, fc3_bias_stream);
    hls_thread_local hls::task t2(conv1_space::conv1, img_stream, conv1_out_stream);
    hls_thread_local hls::task t3(pool1_space::pool1, conv1_out_stream, pool1_out_stream);
    hls_thread_local hls::task t4(conv2_space::conv2, pool1_out_stream, conv2_out_stream);
    hls_thread_local hls::task t5(pool2_space::pool2, conv2_out_stream, pool2_out_stream);
    hls_thread_local hls::task t6(conv3_space::conv3, pool2_out_stream, conv3_out_stream);
    hls_thread_local hls::task t7(conv4_space::conv4, conv3_out_stream, conv4_out_stream);
    hls_thread_local hls::task t8(conv5_space::conv5, conv4_out_stream, conv5_out_stream);
    hls_thread_local hls::task t9(pool3_space::pool3, conv5_out_stream, pool3_out_stream);
    hls_thread_local hls::task t10(fc1_space::fc1, pool3_out_stream, fc1_weight_stream, fc1_bias_stream, fc1_out_stream);
    hls_thread_local hls::task t11(fc2_space::fc2, fc1_out_stream, fc2_weight_stream, fc2_bias_stream, fc2_out_stream);
    hls_thread_local hls::task t12(fc3_space::fc3, fc2_out_stream, fc3_weight_stream, fc3_bias_stream, fc3_out_stream);
    hls_thread_local hls::task t13(top_out_space::top_out, fc3_out_stream, res);
}