#include "top.hpp"

void top(
    unsigned char data[IMAGE_WIDTH],
    int result_x[RESULT_SIZE],
    int result_y[RESULT_SIZE],
    int result_w[RESULT_SIZE],
    int result_h[RESULT_SIZE],
    int *result_size
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi depth=IMAGE_WIDTH port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_x offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_y offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_w offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_h offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=result_size offset=slave bundle=data

    hls_thread_local hls::stream<data_t> in_stream("in_stream");
    hls_thread_local hls::stream<data_t> scale_stream("scale_stream");
    hls_thread_local hls::stream<result_t> result_stream("result_stream");
    
    hls_thread_local hls::task t1(top_in_space::top_in, data, in_stream);
    hls_thread_local hls::task t2(scale_space::scale, in_stream, scale_stream);
    hls_thread_local hls::task t3(vj_space::vj, scale_stream, result_stream);
    hls_thread_local hls::task t4(top_out_space::top_out, result_stream, result_x, result_y, result_w, result_h, result_size);
}