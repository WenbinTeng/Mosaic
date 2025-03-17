#include "layer4_fc.hpp"

/**
 *  IN:     120
 *  WEIGHT: 84*120
 *  BIAS:   84
 *  OUT:    84
 */
void layer4_fc(
    hls::stream<DTYPE>& in_stream,
    hls::stream<DTYPE>& weight_stream,
    hls::stream<DTYPE>& bias_stream,
    hls::stream<DTYPE>& out_stream
) {
    DTYPE in_buff[120];

    for (int c = 0; c < 120; c++) {
        in_buff[c] = in_stream.read();
    }

    DTYPE sum, tmp;

    for (int n = 0; n < 84; n++) {
        sum = 0;
        for (int c = 0; c < 120; c++) {
#pragma HLS PIPELINE
            sum += weight_stream.read() * in_buff[c];
        }
        tmp = sum + bias_stream.read();
        out_stream.write(tmp);
    }
}