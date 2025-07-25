#include "bin_conv.hpp"

namespace bin_conv_space {

inline void _unpack_input(din_t& input, word_t _input[IN_PACKS]) {
#pragma HLS INLINE
    for (int p = 0; p < IN_PACKS; p++) {
#pragma HLS UNROLL
        _input[p] = input.range(p * 8 + 7, p * 8);
    }
}

inline void _pack_output(acc_t _output[OUT_PACKS], dout_t& output) {
#pragma HLS INLINE
    for (int p = 0; p < OUT_PACKS; p++) {
#pragma HLS UNROLL
        output.range(p * 8 + 7, p * 8) = (word_t)_output[p];
    }
}

inline void _init_weight(word_t weight[OUT_CH][K][K]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < K; j++) {
                weight[oc][i][j] = (word_t)INT32_MAX * std::sin(oc * i * j);
            }
        }
    }
}

inline void _init_threshold(threshold_t th_table[OUT_CH]) {
#pragma HLS INLINE
    for (int oc = 0; oc < OUT_CH; oc++) {
        th_table[oc] = 256 * std::sin(oc);
    }
}

inline acc_t _popcount(word_t x) {
#pragma HLS INLINE
    int cnt = 0;
    for (int i = 0; i < 256; i++)
#pragma HLS UNROLL
        cnt = cnt + x[i];
    return cnt;
}

void bin_conv(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    word_t weight[OUT_CH][K][K];
// #pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
#pragma HLS ARRAY_PARTITION variable=weight type=complete dim=1
    _init_weight(weight);

    threshold_t th_table[OUT_CH];
#pragma HLS ARRAY_PARTITION variable=th_table complete dim=0
    _init_threshold(th_table);

    word_t line_buff[K-1][IN_W];
#pragma HLS ARRAY_PARTITION variable=line_buff complete dim=1

    for (int oh = 0; oh < OUT_H; oh++) {
        for (int ow = 0; ow < OUT_W; ow++) {
            
            for (int r = K - 2; r > 0; r--) {
#pragma HLS UNROLL
                line_buff[r][ow] = line_buff[r-1][ow];
            }
            line_buff[0][ow] = in_stream.read();
            
            if (oh >= P && ow >= P) {
                for (int ocbase = 0; ocbase < OUT_CH; ocbase += WORD_LEN) {
                    word_t out_bits = 0;
                    for (int ocp = 0; ocp < WORD_LEN; ocp++) {
#pragma HLS UNROLL
                        acc_t acc = 0;
                        for (int ky = 0; ky < K; ky++) {
#pragma HLS UNROLL
                            word_t pix_word;
                            for (int kx = 0; kx < K; kx++) {
#pragma HLS UNROLL
                                int col = ow - P + kx;
                                if (ky == K - 1) {
                                    pix_word = (col >= 0 && col < IN_W) ? line_buff[0][col] : (word_t)0;
                                } else {
                                    pix_word = (col >= 0 && col < IN_W) ? line_buff[ky][col] : (word_t)0;
                                }
                                word_t w = weight[ocbase + ocp][ky][kx];
                                word_t xnor = ~(pix_word ^ w);
                                acc += _popcount(xnor);
                            }
                        }
                        bool bit = (acc >= th_table[ocbase + ocp]);
                        out_bits[ocp] = bit;
                    }
                    out_stream.write(out_bits);
                }
            }
        }
    }
}

}  // namespace bin_conv_space