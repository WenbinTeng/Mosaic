#include "full5.hpp"

/**
 *  IN:     84
 *  WEIGHT: 10*84
 *  BIAS:   10
 *  OUT:    10
 */
void full5(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[84];
    
    for (int c = 0; c < 84; c++) {
        in_buff[c] = in_stream.read();
    }

    DTYPE sum, tmp;

    for (int n = 0; n < 10; n++) {
        sum = 0;
        for (int c = 0; c < 84; c++) {
            sum += weight_stream.read() * in_buff[c];
        }
        tmp = sum + bias_stream.read();
        out_stream.write(tmp);
    }
}