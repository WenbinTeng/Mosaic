#include "conv1.hpp"

/**
 *  IN:     32*32
 *  WEIGHT: 6*5*5
 *  BIAS:   6
 *  OUT:    6*28*28
 *  STRIDE: 1
 */

namespace conv1_space {

inline void _unpack_input(din_t& input, feature_t& _input) {
#pragma HLS INLINE
    _input = input;
}

inline void _pack_output(acc_t _output[OUT_CH], dout_t& output) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
#pragma HLS UNROLL
        output.range(oc * 8 + 7, oc * 8) = (feature_t)_output[oc];
    }
}

inline void _init_weight(weight_t weight[OUT_CH][K][K]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < K; j++) {
                weight[oc][i][j] = 256 * std::sin(oc * i * j);
            }
        }
    }
}

inline void _init_bias(acc_t bias[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        bias[oc] = 65536 * std::sin(oc);
    }
}

void conv1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Weight ROM ***/
    weight_t weight[OUT_CH][K][K];
#pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
    _init_weight(weight);

    /*** Bias ROM ***/
    acc_t bias[OUT_CH];
#pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    /*** Line buffer ***/
    feature_t line_buff[K][IN_W + K - 1];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1

    /*** Window buffer ***/
    feature_t window_buff[K][K];
#pragma HLS ARRAY_PARTITION variable=window_buff complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H + K - 1; row++) {
        for (int col = 0; col < IN_W + K - 1; col++) {
#pragma HLS PIPELINE

            /*** 1. Update line buffer. ***/
            feature_t pix_in = 0;
            if (row < IN_H && col < IN_W) {
                din_t din = in_stream.read();
                _unpack_input(din, pix_in);
            }
            for (int i = 0; i < K - 1; i++) {
#pragma HLS UNROLL
                line_buff[i][col] = line_buff[i + 1][col];
            }
            line_buff[K - 1][col] = pix_in;

            /*** 2. Shift lines in window buffer. ***/
            for (int i = 0; i < K; i++) {
#pragma HLS UNROLL
                for (int j = 0; j < K - 1; j++) {
#pragma HLS UNROLL
                    window_buff[i][j] = window_buff[i][j + 1];
                }
                window_buff[i][K - 1] = line_buff[i][col];
            }

            /*** 3. Compute partial sums. ***/
            if (row >= K - 1 && col >= K - 1) {

                /* (a). Partial sums buffer. */
                acc_t psum[OUT_CH];
#pragma HLS ARRAY_PARTITION variable=psum complete

                /* (b). Compute sums for all output channels. */
                for (int oc = 0; oc < OUT_CH; oc++) {
#pragma HLS UNROLL
                    acc_t sum = bias[oc];
                    for (int i = 0; i < K; i++) {
#pragma HLS UNROLL
                        for (int j = 0; j < K; j++) {
#pragma HLS UNROLL
                            sum += window_buff[i][j] * weight[oc][i][j];
                        }
                    }
                    psum[oc] = relu(sum);
                }

                /* (c). Write outputs. */
                dout_t dout;
                _pack_output(psum, dout);
                out_stream.write(dout);
            }
        }
    }
}

} // namespace conv1_space