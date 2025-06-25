#include "conv1.hpp"

/**
 *  IN:     32*32
 *  WEIGHT: 6*5*5
 *  BIAS:   6
 *  OUT:    6*28*28
 *  STRIDE: 1
 */

constexpr int IN_CH     = 1;                // input channel
constexpr int OUT_CH    = 6;                // output channel
constexpr int IMG_H     = 32;               // input height
constexpr int IMG_W     = 32;               // input width
constexpr int K         = 5;                // kernel size
constexpr int OUT_H     = IMG_H - K + 1;    // 28, output height
constexpr int OUT_W     = IMG_W - K + 1;    // 28, output width

void conv1(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream,
    const weight_t          weight[OUT_CH][K][K],
    const acc_t             bias[OUT_CH]
) {
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE bram port=weight
#pragma HLS INTERFACE bram port=bias

    /*** Line buffer ***/
    feature_t line_buff[K][IMG_W];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1

    /*** Window buffer ***/
    feature_t window_buff[K][K];
#pragma HLS ARRAY_PARTITION variable=window_buff complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IMG_H + K - 1; row++) {
        for (int col = 0; col < IMG_W + K - 1; col++) {
#pragma HLS PIPELINE

            /*** 1. Update line buffer. ***/
            feature_t pix_in = 0;
            if (row < IMG_H && col < IMG_W) {
                pix_in = in_stream.read();
            }

            /*** 2. Shift lines in window buffer. ***/
            for (int i = 0; i < K; i++) {
#pragma HLS UNROLL
                if (i == K - 1)
                    line_buff[i][col] = pix_in;
                else
                    line_buff[i][col] = line_buff[i + 1][col];
            }

            /*** 3. Compute partial sums. ***/
            if (row >= K - 1 && col >= K - 1) {

                /* (a). Partial sums buffer. */
                acc_t psum[OUT_CH];
#pragma HLS ARRAY_PARTITION variable=psum complete

                /* (b). Compute sums for all output channels. */
                for (int oc = 0; oc < OUT_CH; oc++) {
                    acc_t sum = bias[oc];
                    for (int i = 0; i < K; i++) {
#pragma HLS UNROLL
                        for (int j = 0; j < K; j++) {
#pragma HLS UNROLL
                            sum += window_buff[i][j] * weight[oc][i][j];
                        }
                    }
                    psum[oc] = sum;
                }

                /* (c). Write outputs. */
                for (int oc = 0; oc < OUT_CH; oc++) {
#pragma HLS UNROLL
                    out_stream.write((feature_t)relu(psum[oc]));
                }
            }
        }
    }
}