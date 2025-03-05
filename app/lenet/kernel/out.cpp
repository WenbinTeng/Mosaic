#include "out.hpp"

/**
 *  IN:     84
 *  WEIGHT: 10*84
 *  BIAS:   10
 *  OUT:    10
 */
void out(
    hls::stream<float>& in_stream,
    hls::stream<float>& weight_stream,
    hls::stream<float>& bias_stream,
    hls::stream<float>& out_stream
) {
    float in_buff[84];
    
    for (int c = 0; c < 84; c++) {
        in_buff[c] = in_stream.read();
    }

    float sum, tmp;

    for (int n = 0; n < 10; n++) {
        sum = 0;
        for (int c = 0; c < 84; c++) {
            sum += weight_stream.read() * in_buff[c];
        }
        tmp = sum + bias_stream.read();
        out_stream.write(tmp);
    }
}