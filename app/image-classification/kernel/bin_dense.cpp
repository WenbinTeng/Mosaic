#include "bin_dense.hpp"

namespace bin_dense_space {

inline void _init_weight(word_t weight[OUT_CLASS][IN_SIZE]) {
#pragma HLS INLINE
    for (int i = 0; i < OUT_CLASS; i++) {
        for (int j = 0; j < IN_SIZE; j++) {
            weight[i][j] = 4294967296 * std::sin(i * j);
        }
    }
}

inline void _init_bias(acc_t bias[OUT_CLASS]) {
#pragma HLS INLINE
    for (int i = 0; i < OUT_CLASS; i++) {
        bias[i] = 4294967296 * std::sin(i);
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

void bin_dense(
    hls::stream<din_t> &in_stream,
    hls::stream<dout_t> &out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    word_t weight[OUT_CH][IN_SIZE / WORD_LEN];
#pragma HLS BIND_STORAGE variable=weight type=rom_1p impl=auto
    _init_weight(weight);

    acc_t bias[OUT_CH];
#pragma HLS BIND_STORAGE variable=bias type=rom_1p impl=auto
    _init_bias(bias);

    for (int oc = 0; oc < OUT_CLASS; oc++) {
        word_t acc = bias[oc];
        for (int ip = 0; ip < IN_SIZE / WORD_LEN; ip++) {
#pragma HLS PIPELINE
            din_t din = in_stream.read();
            word_t w = weight[oc][ip];
            word_t xnor = ~(din ^ w);
            acc += _popcount(xnor);
        }
        out_stream.write(acc);
    }

}

}  // namespace bin_dense_space