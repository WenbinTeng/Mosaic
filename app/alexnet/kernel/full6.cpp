#include "full6.hpp"

/**
 *  IN:     9216
 *  WEIGHT: 4096*9216
 *  BIAS:   4096
 *  OUT:    4096
 */
void full6(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE sum, tmp;

    for (int n = 0; n < 4096; n++) {
        sum = 0;
        for (int c = 0; c < 9216; c++) {
#pragma HLS PIPELINE
            DTYPE weight_buff = weight_stream.read();
            DTYPE in_buff = in_stream.read();
            sum += weight_buff * in_buff;
        }
        DTYPE bias_buff = bias_stream.read();
        tmp = sum + bias_buff;
        out_stream.write(tmp);
    }
}