#include "fc3.hpp"

/**
 *  IN:     84
 *  WEIGHT: 10*84
 *  BIAS:   10
 *  OUT:    10
 */

namespace fc3_space {

inline void _unpack_input(din_t& input, feature_t _input[PREV_PAR]) {
#pragma HLS INLINE
    for (int i = 0; i < PREV_PAR; i++) {
        _input[i] = input.range(i * 8 + 7, i * 8);
    }
}

inline void _pack_output(acc_t _output[PAR], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = (feature_t)_output[p];
    }
}

inline void _init_weight(weight_t weight[OUT_SIZE][IN_SIZE]) {
#pragma HLS INLINE
    for (int i = 0; i < OUT_SIZE; i++) {
        for (int j = 0; j < IN_SIZE; j++) {
            weight[i][j] = 256 * std::sin(i * j);
        }
    }
}

inline void _init_bias(acc_t bias[OUT_SIZE]) {
#pragma HLS INLINE
    for (int i = 0; i < OUT_SIZE; i++) {
        bias[i] = 65536 * std::sin(i);
    }
}

void fc3(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Weight ROM ***/
    weight_t weight[OUT_SIZE][IN_SIZE];
#pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
    _init_weight(weight);

    /*** Bias ROM ***/
    acc_t bias[OUT_SIZE];
#pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    /*** Input buffer ***/
    feature_t in_buff[IN_SIZE];
#pragma HLS ARRAY_PARTITION variable=in_buff cyclic factor=PREV_PAR
    for (int base = 0; base < IN_SIZE; base += PREV_PAR) {
#pragma HLS PIPELINE
        din_t din = in_stream.read();
        _unpack_input(din, &in_buff[base]);
    }

    /*** Main loop ***/
    for (int pbase = 0; pbase < OUT_SIZE; pbase += PAR) {
#pragma HLS PIPELINE

        /* (a). Partial sums buffer. */
        acc_t psum[PAR];
#pragma HLS ARRAY_PARTITION variable=psum type=complete
        for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
            psum[p] = bias[pbase + p];
        }

        /* (b). Compute sums at parallel factor. */
        for (int ic = 0; ic < IN_SIZE; ic++) {
#pragma HLS UNROLL
            feature_t fm = in_buff[ic];
            for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                weight_t w = weight[pbase + p][ic];
                psum[p] += fm * w;
            }
        }

        /* (c). Write outputs. */
        dout_t dout;
        _pack_output(psum, dout);
        out_stream.write(dout);
    }
}

}  // namespace fc3_space