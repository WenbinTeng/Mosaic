#include "conv2.hpp"

/**
 *  IN:      96*27*27
 *  WEIGHT:  256*96*5*5
 *  BIAS:    256
 *  OUT:     256*27*27
 *  STRIDE:  1
 *  PADDING: 2
 */

namespace conv2_space {

inline void _unpack_input(din_t& input, feature_t _input[PREV_PAR]) {
#pragma HLS INLINE
    for (int ic = 0; ic < PREV_PAR; ic++) {
#pragma HLS UNROLL
        _input[ic] = input.range(ic * 8 + 7, ic * 8);
    }
}

inline void _pack_output(acc_t _output[PAR], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = (feature_t)_output[p];
    }
}

inline void _init_weight(weight_t weight[OUT_CH][IN_CH][K][K]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        for (int ic = 0; ic < IN_CH; ic++) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j++) {
                    weight[oc][ic][i][j] = 256 * std::sin(oc * ic * i * j);
                }
            }
        }
    }
}

inline void _init_bias(bias_t bias[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        bias[oc] = 256 * std::sin(oc);
    }
}

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
    bias_t bias[OUT_CH];
#pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    /*** Line buffer ***/
    feature_t line_buff[PREV_PAR][K][IN_W + K - 1];
#pragma HLS ARRAY_PARTITION variable=line_buff type=complete dim=1
#pragma HLS ARRAY_PARTITION variable=line_buff type=complete dim=2

    /*** Window buffer ***/
    feature_t window_buff[PREV_PAR][K][K];
#pragma HLS ARRAY_PARTITION variable=window_buff type=complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H + 2 * P + K - 1; row++) {
        for (int col = 0; col < IN_W + 2 * P + K - 1; col++) {

            /*** 1. Update line buffer. ***/
            feature_t pix_in[PREV_PAR];
#pragma HLS ARRAY_PARTITION variable=pix_in type=complete
            bool out_pad_r = (row >= P) && (row < P + IN_H);
            bool out_pad_w = (col >= P) && (col < P + IN_W);
            if (out_pad_r && out_pad_w) {
                din_t din = in_stream.read();
                _unpack_input(din, pix_in);
            } else {
                for (int ic = 0; ic < PREV_PAR; ic++) {
#pragma HLS UNROLL
                    pix_in[ic] = 0;
                }
            }
            for (int ic = 0; ic < PREV_PAR; ic++) {
#pragma HLS UNROLL
                for (int r = K - 1; r > 0; r--) {
#pragma HLS UNROLL
                    line_buff[ic][r][col] = line_buff[ic][r - 1][col];
                }
                line_buff[ic][0][col] = pix_in[ic];
            }

            /*** 2. Shift lines in window buffer. ***/
            for (int ic = 0; ic < PREV_PAR; ic++) {
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

            bool window_valid = row >= K - 1 && col >= K - 1;
            bool stride_valid = (((row - (K - 1)) & (S - 1)) == 0) &&
                                (((col - (K - 1)) & (S - 1)) == 0);

            /*** 3. Compute partial sums. ***/
            for (int pbase = 0; pbase < OUT_CH; pbase += PAR) {
#pragma HLS PIPELINE

                /* (a). Partial sums buffer. */
                acc_t psum[PAR];
#pragma HLS ARRAY_PARTITION variable=psum type=complete
                for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                    psum[p] = bias[pbase + p];
                }

                /* (b). Compute sums for all output channels. */
                for (int ic = 0; ic < PREV_PAR; ic++) {
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
                if (window_valid && stride_valid) {
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