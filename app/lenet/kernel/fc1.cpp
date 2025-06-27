#include "fc1.hpp"

/**
 *  IN:     400
 *  WEIGHT: 400*120
 *  BIAS:   120
 *  OUT:    120
 */

void fc1(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream
) {
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Weight ROM ***/
    weight_t weight[OUT_SIZE][IN_SIZE] = { {1,2,3} };
#pragma HLS BIND_STORAGE variable=weight type=ROM_NP impl=bram

    /*** Bias ROM ***/
    acc_t bias[OUT_SIZE] = {};
#pragma HLS BIND_STORAGE variable=bias type=ROM_1P impl=bram

    /*** Input buffer ***/
    feature_t in_buff[IN_SIZE];
#pragma HLS ARRAY_PARTITION variable=in_buff cyclic factor=16
    for (int i = 0; i < IN_SIZE; i++) {
#pragma HLS PIPELINE
        in_buff[i] = in_stream.read();
    }

    /*** Main loop ***/
    for (int pbase = 0; pbase < OUT_SIZE; pbase += PAR) {
#pragma HLS PIPELINE

        /* (a). Partial sums buffer. */
        acc_t psum[PAR];
#pragma HLS ARRAY_PARTITION variable=psum complete
        for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
            psum[p] = bias[pbase + p];
        }

        /* (b). Compute sums at parallel factor. */
        for (int ic = 0; ic < IN_SIZE; ic++) {
#pragma HLS PIPELINE
            feature_t fm = in_buff[ic];
            for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                weight_t w = weight[pbase + p][ic];
                psum[p] += fm * w;
            }
        }

        /* (c). Write outputs. */
        for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
            out_stream.write((feature_t)relu(psum[p]));
        }
    }
}