#include "conv2.hpp"

/**
 *  IN:     6*14*14
 *  WEIGHT: 16*6*5*5
 *  BIAS:   16
 *  OUT:    16*10*10
 *  STRIDE: 1
 */

namespace conv2_space {

void conv2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Weight ROM ***/
    weight_t weight[OUT_CH][IN_CH][K][K];
#pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
    _init_weight(weight);

    /*** Bias ROM ***/
    acc_t bias[OUT_CH];
#pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    /*** Line buffer ***/
    feature_t line_buff[IN_CH][K][IN_W + K - 1];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=2

    /*** Window buffer ***/
    feature_t window_buff[IN_CH][K][K];
#pragma HLS ARRAY_PARTITION variable=window_buff complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H + K - 1; row++) {
        for (int col = 0; col < IN_W + K - 1; col++) {
#pragma HLS PIPELINE

            /*** 1. Update line buffer. ***/
            feature_t pix_in[IN_CH];
#pragma HLS ARRAY_PARTITION variable=pix_in complete
            if (row < IN_H && col < IN_W) {
                din_t din = in_stream.read();
                _unpack_input(din, pix_in);
            } else {
                for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                    pix_in[ic] = 0;
                }
            }
            for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                for (int r = K - 1; r > 0; r--) {
#pragma HLS UNROLL
                    line_buff[ic][r][col] = line_buff[ic][r - 1][col];
                }
                line_buff[ic][0][col] = pix_in[ic];
            }

            /*** 2. Shift lines in window buffer. ***/
            for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                for (int r = 0; r < K; r++) {
#pragma HLS UNROLL
                    for (int c = 0; c < K - 1; c++) {
#pragma HLS UNROLL
                        window_buff[ic][r][c] = window_buff[ic][r][c + 1];
                    }
                    window_buff[ic][r][K - 1] = line_buff[ic][r][col];
                }
            }

            /*** 3. Compute partial sums. ***/
            if (row >= K - 1 && col >= K - 1) {
                for (int pbase = 0; pbase < OUT_CH; pbase += PAR) {
#pragma HLS UNROLL

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
                        for (int r = 0; r < K; r++) {
#pragma HLS UNROLL
                            for (int c = 0; c < K; c++) {
#pragma HLS UNROLL
                                feature_t fm = window_buff[ic][r][c];
                                for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                                    weight_t w = weight[pbase + p][ic][r][c];
                                    psum[p] += fm * w;
                                }
                            }
                        }
                    }

                    /* (c). Write outputs. */
                    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                        psum[p] = relu(psum[p]);
                    }
                    dout_t dout;
                    _pack_output(psum, dout);
                    out_stream.write(dout);
                }
            }
        }
    }
}

} // namespace conv2_space