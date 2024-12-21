#include "top.hpp"

int face_detect_kernel_0(unsigned char data[IMAGE_WIDTH],
                 int result_x[RESULT_SIZE],
                 int result_y[RESULT_SIZE],
                 int result_w[RESULT_SIZE],
                 int result_h[RESULT_SIZE],
                 int *result_size) {

#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=IMAGE_WIDTH port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_x offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_y offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_w offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_h offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=result_size offset=slave bundle=data

    face_detect(data, result_x, result_y, result_w, result_h, result_size);
    return 0;
}