#include "fp_conv.hpp"

/**
 *  IN:      3*32*32
 *  WEIGHT:  128*3*3
 *  BIAS:    128
 *  OUT:     128*32*32
 *  STRIDE:  1
 *  PADDING: 1
 */

namespace fp_conv_space {

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

inline void _init_threshold(threshold_t th_table[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        th_table[oc] = 256 * std::sin(oc);
    }
}

void fp_conv(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    weight_t weight[OUT_CH][IN_CH][K][K];
// #pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
#pragma HLS ARRAY_PARTITION variable=weight type=complete dim=1

    bias_t bias[OUT_CH];
// #pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    threshold_t th_table[OUT_CH];
#pragma HLS ARRAY_PARTITION variable=th_table type=complete dim=0
    _init_threshold(th_table);

    feature_t line_buffer[IN_CH][K-1][IN_W];
#pragma HLS ARRAY_PARTITION variable=line_buffer type=complete dim=1

    for (int oh = 0; oh < OUT_H; oh++) {
        for (int ow = 0; ow < OUT_W; ow++) {

            for (int ic = 0; ic < IN_CH; ic++) {
#pragma HLS UNROLL
                for (int row = K-2; row > 0; row--) {
#pragma HLS UNROLL
                    line_buffer[ic][row][ow] = line_buffer[ic][row-1][ow];
                }
                din_t new_pix = in_stream.read();
                line_buffer[ic][0][ow] = new_pix;
            }

            if (oh >= P && ow >= P) {
                acc_t out_pack[PAR];
#pragma HLS ARRAY_PARTITION variable=out_pack complete dim=0

                for (int p = 0; p < PAR; ++p) {
#pragma HLS UNROLL
                    out_pack[p] = bias[(ow * OUT_CH + oh * OUT_CH * IN_W + p) % OUT_CH];
                }

                for (int ic = 0; ic < IN_CH; ic++) {
                    for (int ky = 0; ky < K; ky++) {
                        for (int kx = 0; kx < K; kx++) {
#pragma HLS PIPELINE
                            feature_t pix_val;
                            int win_row = ky == K - 1 ? 0 : ky;
                            int win_col = ow + kx - P;
                            if (win_col < 0 || win_col >= IN_W)
                                pix_val = 0;
                            else if (ky == K-1)
                                pix_val = line_buffer[ic][ky-1][win_col];
                            else
                                pix_val = line_buffer[ic][win_row][win_col];

                            for (int p = 0; p < PAR; ++p) {
#pragma HLS UNROLL
                                int oc = ((ow * OUT_CH + p) % OUT_CH) + (oh % (OUT_CH / PAR)) * PAR;
                                feature_t w = weight[oc][ic][ky][kx];
                                out_pack[p] += pix_val * w;
                            }
                        }
                    }
                }

                for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
                    int oc = ((ow * OUT_CH + p) % OUT_CH) + (oh % (OUT_CH / PAR)) * PAR;
                    out_stream.write(out_pack[p] >= th_table[oc]);
                }
            }
        }
    }
}

}  // namespace fp_conv_space