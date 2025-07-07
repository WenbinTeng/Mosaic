#include "fc1.hpp"

/**
 *  IN:     9216
 *  WEIGHT: 4096*9216
 *  BIAS:   4096
 *  OUT:    4096
 */

namespace fc1_space {

inline void _unpack_input(din_t& input, feature_t _input[PREV_PAR]) {
#pragma HLS INLINE
    for (int i = 0; i < PREV_PAR; i++) {
        _input[i] = input.range(i * 8 + 7, i * 8);
    }
}

void fc1(
    hls::stream<din_t>& in_stream,
    hls::stream<weight_t>& weight_stream,
    hls::stream<bias_t>& bias_stream,
    hls::stream<dout_t>& out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=weight_stream
#pragma HLS INTERFACE axis port=bias_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Input stationary ***/
    feature_t in_buff[IN_SIZE];
#pragma HLS BIND_STORAGE variable=in_buff type=ram_1p impl=auto
#pragma HLS ARRAY_PARTITION variable=in_buff type=cyclic factor=PREV_PAR
    for (int base = 0; base < IN_SIZE; base += PREV_PAR) {
#pragma HLS PIPELINE
        din_t din = in_stream.read();
        _unpack_input(din, &in_buff[base]);
    }

    /*** Main Loop ***/
    for (int oc = 0; oc < OUT_SIZE; oc++) {

        /*** 1. Initialize accumulation. ***/
        acc_t acc = bias_stream.read();

        /*** 2. Compute chunks. ***/
        for (int pbase = 0; pbase < IN_SIZE; pbase += PAR) {
#pragma HLS PIPELINE

            /* (a). Read chunks. */
            feature_t chunk_x[PAR];
            weight_t  chunk_w[PAR];
            for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                chunk_x[p] = in_buff[pbase + p];
                chunk_w[p] = weight_stream.read();
            }

            /* (b). MAC. */
            for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                acc += chunk_w[p] * chunk_x[p];
            }
        }

        /*** 3. Write outputs. ***/
        out_stream.write(relu(acc));
    }
}

} // namespace fc1_space