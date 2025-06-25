#include "fc2.hpp"

/**
 *  IN:     120
 *  WEIGHT: 120*84
 *  BIAS:   84
 *  OUT:    84
 */

constexpr int IN_SIZE   = 120;  // input size
constexpr int OUT_SIZE  = 84;   // output size
constexpr int PAR       = 12;   // parallel factor

void fc2(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream,
    const weight_t          weight[IN_SIZE][OUT_SIZE],
    const acc_t             bias[OUT_SIZE]
) {
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE bram port=weight
#pragma HLS INTERFACE bram port=bias
#pragma HLS ARRAY_PARTITION variable=weight complete dim=1

    /*** Input buffer ***/
    feature_t in_buff[IN_SIZE];
#pragma HLS ARRAY_PARTITION variable=in_buff cyclic factor=12
    for (int i = 0; i < IN_SIZE; i++) {
#pragma HLS PIPELINE II=1
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
                weight_t w = weight[ic][pbase + p];
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