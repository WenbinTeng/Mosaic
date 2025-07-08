#include "top.hpp"

void top(
    frames_t frames[MAX_HEIGHT][MAX_WIDTH],
    velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=frames offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=outputs offset=slave bundle=data
#pragma HLS DATAFLOW

    hls_thread_local hls::stream<input_t> frame1_stream("frame1_stream");
    hls_thread_local hls::stream<input_t> frame2_stream("frame2_stream");
    hls_thread_local hls::stream<input_t> frame3_stream_a("frame3_stream_a");
    hls_thread_local hls::stream<input_t> frame3_stream_b("frame3_stream_b");
    hls_thread_local hls::stream<input_t> frame4_stream("frame4_stream");
    hls_thread_local hls::stream<input_t> frame5_stream("frame5_stream");
    hls_thread_local hls::stream<pixel_t> grad_x_stream("grad_x_stream");
    hls_thread_local hls::stream<pixel_t> grad_y_stream("grad_y_stream");
    hls_thread_local hls::stream<pixel_t> grad_z_stream("grad_z_stream");
    hls_thread_local hls::stream<gradient_t> y_filt_grad_stream("y_filt_grad_stream");
    hls_thread_local hls::stream<gradient_t> filt_grad_stream("filt_grad_stream");
    hls_thread_local hls::stream<outer_t> outer_stream("outer_stream");
    hls_thread_local hls::stream<tensor_t> ten_y_stream("ten_y_stream");
    hls_thread_local hls::stream<tensor_t> ten_stream("ten_stream");
    hls_thread_local hls::stream<velocity_t> out_stream("out_stream");

    hls_thread_local hls::task t1(
        top_in_space::top_in, frames,
        frame1_stream,
        frame2_stream,
        frame3_stream_a,
        frame3_stream_b,
        frame4_stream,
        frame5_stream);
    hls_thread_local hls::task t2(grad_xy_calc_space::grad_xy_calc, frame3_stream_a, grad_x_stream, grad_y_stream);
    hls_thread_local hls::task t3(grad_z_calc_space::grad_z_calc, frame1_stream, frame2_stream, frame3_stream_b, frame4_stream, frame5_stream, grad_z_stream);
    hls_thread_local hls::task t4(grad_wgt_y_space::grad_wgt_y, grad_x_stream, grad_y_stream, grad_z_stream, y_filt_grad_stream);
    hls_thread_local hls::task t5(grad_wgt_x_space::grad_wgt_x, y_filt_grad_stream, filt_grad_stream);
    hls_thread_local hls::task t6(outer_product_space::outer_product, filt_grad_stream, outer_stream);
    hls_thread_local hls::task t7(ten_wgt_y_space::ten_wgt_y, outer_stream, ten_y_stream);
    hls_thread_local hls::task t8(ten_wgt_x_space::ten_wgt_x, ten_y_stream, ten_stream);
    hls_thread_local hls::task t9(flow_calc_space::flow_calc, ten_stream, out_stream);
    hls_thread_local hls::task t10(top_out_space::top_out, out_stream, outputs);
}