#include "conv2.hpp"

/**
 *  IN:     6*14*14
 *  WEIGHT: 16*6*5*5
 *  BIAS:   16
 *  OUT:    16*10*10
 *  STRIDE: 1
 */

constexpr int IN_CH     = 6;            // input channel
constexpr int OUT_CH    = 16;           // output channel
constexpr int IN_H      = 14;           // input height
constexpr int IN_W      = 14;           // input width
constexpr int K         = 5;            // kernel size
constexpr int OUT_H     = IN_H - K + 1; // 28, output height
constexpr int OUT_W     = IN_W - K + 1; // 28, output width
constexpr int PAR       = 8;            // parallel factor

void conv2(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& weight_stream,
    const weight_t          weight[OUT_CH][IN_CH][K][K],
    const acc_t             bias[OUT_CH]
) {
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE bram port=weight
#pragma HLS INTERFACE bram port=bias

    /*** Line buffer ***/
    feature_t line_buff[IN_CH][K][IN_W];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=2

    /*** Window buffer ***/
    feature_t window_buff[IN_CH][K][K];
#pragma HLS ARRAY_PARTITION variable=window_buff complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H + K - 1; row++) {
        for (int col = 0; col < IN_W + K -1; col++) {
#pragma HLS PIPELINE

            /*** 1. Update line buffer. ***/
            feature_t pix_in[IN_CH];
#pragma HLS ARRAY_PARTITION variable=pix_in complete
            if (row < IN_H && col < IN_W) {
                for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                    pix_in[ic] = in_stream.read();
                }
            } else {
                for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                    pix_in[ic] = 0;
                }
            }

            /*** 2. Shift lines in window buffer. ***/
            for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                for (int i = 0; i < k; i++) {
#pragma HLS UNROLL
                    if (i == K - 1)
                        line_buff[ic][i][col] = pix_in[ic];
                    else
                        line_buff[ic][i][col] = line_buff[ic][i + 1][col];
                }
            }

            /*** 3. Compute partial sums. ***/
            if (row >= K - 1 && col >= K - 1) {
                for (int pbase = 0; pbase < OUT_CH; pbase += PAR) {

                    /* (a). Partial sums buffer. */
                    acc_t psum[PAR];
#pragma HLS ARRAY_PARTITION variable=psum complete
                    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                        psum[p] = bias[pbase + p];
                    }

                    /* (b). Compute sums for all output channels. */
                    for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                        for (int i = 0; i < K; i++) {
#pragma HLS UNROLL
                            for (int j = 0; j < K; j++) {
#pragma HLS UNROLL
                                feature_t fm = window_buff[ic][i][j];
                                for (int p = 0; p < OUT_CH; p++) {
#pragma HLS UNROLL
                                    weight_t w = weight[pbase + p][ic][i][j];
                                    psum[p] += fm * w;
                                }
                            }
                        }
                    }

                    /* (c). Write outputs. */
                    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                        out_stream.write((feature_t)relu(psum[p]));
                    }
                }
            }
        }
    }
}