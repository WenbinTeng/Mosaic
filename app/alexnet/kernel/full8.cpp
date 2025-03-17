#include "full8.hpp"

/**
 *  IN:     4096
 *  WEIGHT: 4096*1000
 *  BIAS:   1000
 *  OUT:    1000
 */
void full8(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE sum, tmp;

    for (int n = 0; n < 1000; n++) {
        sum = 0;
        for (int c = 0; c < 4096; c++) {
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