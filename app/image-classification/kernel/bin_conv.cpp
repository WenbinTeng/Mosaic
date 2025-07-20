#include "bin_conv.hpp"

namespace bin_conv_space {

inline void _unpack_input(din_t& input, feature_t _input[PREV_PAR]) {
#pragma HLS INLINE
    for (int p = 0; p < PREV_PAR; p++) {
#pragma HLS UNROLL
        _input[p] = input.range(p * 8 + 7, p * 8);
    }
}

inline void _pack_output(acc_t _output[PAR], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < PAR; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = (feature_t)_output[p];
    }
}

inline void _init_weight(word_t weight[OUT_CH][IN_PACKS][K][K]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        for (int ic = 0; ic < IN_PACKS; ic++) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < K; j++) {
                    weight[oc][ic][i][j] = 4294967296 * std::sin(oc * ic * i * j);
                }
            }
        }
    }
}

inline void _init_bias(acc_t bias[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        bias[oc] = 4294967296 * std::sin(oc);
    }
}

inline void _init_threshold(acc_t th_table[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        th_table[oc] = 256 * std::sin(oc);
    }
}

inline int _popcount(word_t x) {
#pragma HLS INLINE
    int cnt = 0;
    for (int i = 0; i < 256; i++)
#pragma HLS UNROLL
        cnt = cnt + x[i];
    return cnt;
}

inline void _bn_sign(acc_t acc, int ch) -> bool {
#pragma HLS INLINE
    threshold_t th_table[OUT_CH];
    return acc >= th_table[ch];
}

void bin_conv(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    word_t weight[OUT_CH][IN_PACKS][K][K];
#pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
    _init_weight(weight);

    acc_t bias[OUT_CH];
#pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    acc_t th_table[OUT_CH];
#pragma HLS BIND_STORAGE variable=th_table type=rom_1p impl=auto
    _init_threshold(th_table);

    word_t line_buff[K-1][IMG_W];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1

    for (int oh = 0; oh < OUT_H; oh++) {
        for (int ow = 0; ow < OUT_W; ow++) {
#pragma HLS PIPELINE

            for (int r = K - 2; r > 0; r--) {
#pragma HLS UNROLL
                line_buff[r][ow] = line_buff[r-1][ow];
            }
            line_buff[0][ow] = in_stream.read();
            
            if (oh >= P && ow >= P) {
                word_t out_bits = 0;
                for (int oc = 0; oc < OUT_CH; oc++) {
#pragma HLS UNROLL factor=WORD_LEN
                    acc_t acc = bias[oc];
                    for (int ky = 0; ky < K; ky++) {
                        word_t pix_word;
                        for (int kx = 0; kx < K; kx++) {
                            int col = ow - P + kx;
                            if (ky == K - 1) {
                                pix_word = (col >= 0 && col < IMG_W) ? line_buff[0][col] : 0;
                            } else {
                                pix_word = (col >= 0 && col < IMG_W) ? line_buff[ky][col] : 0;
                            }
                            word_t w = weight[oc][ky][kx];
                            word_t xnor = ~(pix_word ^ w);
                            acc += _popcount(xnor);
                        }
                    }
                    bool bit = _bn_sign(acc);
                    out_bits[oc % WORD_LEN] = bit;
                    if (oc % WORD_LEN == WORD_LEN - 1) {
                        out_stream.write(out_bits);
                        out_bits = 0;
                    }
                }
            }
        }
    }
}

}  // namespace bin_conv_space