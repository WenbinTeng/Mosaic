#include "pool1.hpp"

/**
 *  IN:     6*28*28
 *  KERNEL: 2*2
 *  OUT:    6*14*14
 */

void pool1(
    hls::stream<din_t>& in_stream,
    hls::stream<dout_t>& out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Line buffer ***/
    feature_t line_buff[CH][K][IN_W + 1];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=2

    /*** Window buffer ***/
    feature_t window_buff[K][K][CH];
#pragma HLS ARRAY_PARTITION variable=window_buff complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H + 1; row++) {
        for (int col = 0; col < IN_W + 1; col++) {
#pragma HLS PIPELINE

            /*** 1. Update line buffer (per output channel). ***/
            feature_t pix_in[CH];
#pragma HLS ARRAY_PARTITION variable=pix_in complete
            if (row < IN_H && col < IN_W) {
                din_t din = in_stream.read();
                _unpack_input(din, pix_in);
            } else {
                for (int c = 0; c < CH; c++) {
#pragma HLS UNROLL
                    pix_in[c] = 0;
                }
            }
            for (int c = 0; c < CH; c++) {
#pragma HLS UNROLL
                line_buff[c][1][col] = line_buff[c][0][col];
                line_buff[c][0][col] = pix_in[c];
            }

            /*** 2. Shift window ***/
            for (int c = 0; c < CH; c++) {
#pragma HLS UNROLL
                window_buff[0][0][c] = window_buff[0][1][c];
                window_buff[1][0][c] = window_buff[1][1][c];
                window_buff[0][1][c] = line_buff[c][0][col];
                window_buff[1][1][c] = line_buff[c][1][col];
            }

            /*** 3. Output max values ***/
            if ((row & 1) && (col & 1)) {

                feature_t maxv[CH];
#pragma HLS ARRAY_PARTITION variable=maxv complete

                for (int c = 0; c < CH; c++) {
#pragma HLS UNROLL
                    acc_t sum = (window_buff[0][0] + window_buff[0][1] +
                                 window_buff[1][0] + window_buff[1][1]);
                    maxv[c] = sum / 4;
                }

                dout_t dout;
                _pack_output(maxv, dout);
                out_stream.write(dout);
            }
        }
    }
}