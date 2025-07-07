#include "top.hpp"

void top(
    bit32 input[3 * NUM_3D_TRI],
    bit32 output[NUM_FB]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=data
#pragma HLS DATAFLOW

    hls_thread_local hls::stream<triangle_3d_t> triangle_3d_stream("triangle_3d_stream");
    hls_thread_local hls::stream<triangle_2d_t> triangle_2d_stream("triangle_2d_stream");
    hls_thread_local hls::stream<triangle_2d_t> triangle_2d_copy_stream("triangle_2d_copy_stream");
    hls_thread_local hls::stream<triangle_info_t> triangle_info_stream("triangle_info_stream");
    hls_thread_local hls::stream<fragment_t> fragment_stream("fragment_stream");
    hls_thread_local hls::stream<pixel_t> pixel_stream("pixel_stream");
    hls_thread_local hls::stream<bit8> frame_stream("frame_stream");

    hls_thread_local hls::task t1(top_in_space::top_in, input, triangle_3d_stream);
    hls_thread_local hls::task t2(projection_space::projection, triangle_3d_stream, triangle_2d_stream);
    hls_thread_local hls::task t3(rasterization1_space::rasterization1, triangle_2d_stream, triangle_2d_copy_stream, triangle_info_stream);
    hls_thread_local hls::task t4(rasterization2_space::rasterization2, triangle_2d_copy_stream, triangle_info_stream, fragment_stream);
    hls_thread_local hls::task t5(zculling_space::zculling, fragment_stream, pixel_stream);
    hls_thread_local hls::task t6(coloring_space::coloring, pixel_stream, frame_stream);
    hls_thread_local hls::task t7(top_out_space::top_out, frame_stream, output);
}