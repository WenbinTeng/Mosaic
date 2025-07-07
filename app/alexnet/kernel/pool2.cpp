#include "pool2.hpp"

/**
 *  IN:     256*27*27
 *  KERNEL: 3*3
 *  OUT:    256*13*13
 *  STRIDE: 2
 */

namespace pool2_space {

inline void _unpack_input(din_t& input, feature_t _input[PAR]) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        _input[p] = input.range(p * 8 + 7, p * 8);
    }
}

inline void _pack_output(feature_t _output[PAR], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = _output[p];
    }
}

inline feature_t _max3(feature_t a, feature_t b, feature_t c) {
#pragma HLS INLINE
    feature_t m0 = (a > b) ? a : b;
    return (m0 > c) ? m0 : c;
}

void pool2(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Line buffer ***/
    feature_t line_buff[PAR][K][IN_W + K - 1];
#pragma HLS ARRAY_PARTITION variable=line_buff type=complete dim=1
#pragma HLS ARRAY_PARTITION variable=line_buff type=complete dim=2

    /*** Window buffer ***/
    feature_t window_buff[K][K][PAR];
#pragma HLS ARRAY_PARTITION variable=window_buff type=complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H + K - 1; row++) {
        for (int col = 0; col < IN_W + K - 1; col++) {
            for (int pbase = 0; pbase < CH; pbase += PAR) {
#pragma HLS PIPELINE

                /*** 1. Update line buffer (per output channel). ***/
                feature_t pix_in[PAR];
#pragma HLS ARRAY_PARTITION variable=pix_in type=complete
                if (row < IN_H && col < IN_W) {
                    din_t din = in_stream.read();
                    _unpack_input(din, pix_in);
                } else {
                    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                        pix_in[p] = 0;
                    }
                }
                for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                    line_buff[p][2][col] = line_buff[p][1][col];
                    line_buff[p][1][col] = line_buff[p][0][col];
                    line_buff[p][0][col] = pix_in[p];
                }

                /*** 2. Shift window ***/
                for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                    window_buff[0][0][p] = window_buff[0][1][p];
                    window_buff[0][1][p] = window_buff[0][2][p];
                    window_buff[1][0][p] = window_buff[1][1][p];
                    window_buff[1][1][p] = window_buff[1][2][p];
                    window_buff[2][0][p] = window_buff[2][1][p];
                    window_buff[2][1][p] = window_buff[2][2][p];
                    window_buff[0][2][p] = line_buff[p][0][col];
                    window_buff[1][2][p] = line_buff[p][1][col];
                    window_buff[2][2][p] = line_buff[p][2][col];
                }

                /*** 3. Output max values ***/
                if ((row & 1) && (col & 1)) {
                    feature_t maxv[PAR];
#pragma HLS ARRAY_PARTITION variable=maxv type=complete

                    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                        feature_t m0 = _max3(window_buff[0][0][p], window_buff[0][1][p], window_buff[0][2][p]);
                        feature_t m1 = _max3(window_buff[1][0][p], window_buff[1][1][p], window_buff[1][2][p]);
                        feature_t m2 = _max3(window_buff[2][0][p], window_buff[2][1][p], window_buff[2][2][p]);
                        maxv[p] = _max3(m0, m1, m2);
                    }

                    dout_t dout;
                    _pack_output(maxv, dout);
                    out_stream.write(dout);
                }
            }
        }
    }
}

}  // namespace pool2_space