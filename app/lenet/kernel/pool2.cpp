#include "pool2.hpp"

/**
 *  IN:     16*10*10
 *  OUT:    16*5*5
 */

constexpr int OUT_CH    = 16;       // output channel
constexpr int IN_H      = 10;       // input height
constexpr int IN_W      = 10;       // input width
constexpr int K         = 2;        // kernel size
constexpr int OUT_H     = IN_H / K; // 5, output height
constexpr int OUT_W     = IN_W / K; // 5, output width

void pool2(
    hls::stream<feature_t>& in_stream,
    hls::stream<feature_t>& out_stream
) {
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    /*** Line buffer ***/
    feature_t line_buff[OUT_CH][K][IN_W];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=2

    /*** Window buffer ***/
    feature_t window_buff[K][K][OUT_CH];
#pragma HLS ARRAY_PARTITION variable=window_buff complete dim=0

    /*** Main loop ***/
    for (int row = 0; row < IN_H; row++) {
        for (int col = 0; col < IN_W; col++) {
#pragma HLS PIPELINE

            /*** 1. Update line buffer (per output channel). ***/
            feature_t pix_in[OUT_CH];
#pragma HLS ARRAY_PARTITION variable=pix_in complete
            if (row < IN_H && col < IN_W) {
                for (int c = 0; c < OUT_CH; c++) {
#pragma HLS UNROLL
                    pix_in[c] = in_stream.read();
                }
            } else {
                for (int c = 0; c < OUT_CH; c++) {
                    pix_in[c] = 0;
                }
            }
            for (int c = 0; c < OUT_CH; c++) {
#pragma HLS UNROLL
                line_buff[c][1][col] = line_buff[c][0][col];
                line_buff[c][0][col] = pix_in[c];
            }

            /*** 2. Shift window ***/
            for (int c = 0; c < OUT_CH; c++) {
#pragma HLS UNROLL
                window_buff[0][0][c] = window_buff[0][1][c];
                window_buff[1][0][c] = window_buff[1][1][c];
                window_buff[0][1][c] = line_buff[c][0][col];
                window_buff[1][1][c] = line_buff[c][1][col];
            }

            /*** 3. Output max values ***/
            if ((row & 1) && (col & 1)) {
                for (int c = 0; c < OUT_CH; c++) {
#pragma HLS UNROLL
                    feature_t m0 = (window_buff[0][0][c] > window_buff[0][1][c]) ? window_buff[0][0][c] : window_buff[0][1][c];
                    feature_t m1 = (window_buff[1][0][c] > window_buff[1][1][c]) ? window_buff[1][0][c] : window_buff[1][1][c];
                    feature_t maxv = (m0 > m1) ? m0 : m1;
                    out_stream.write(maxv); 
                }
            }
        }
    }
}