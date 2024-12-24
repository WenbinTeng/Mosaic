#include "top.hpp"

int image_cls_kernel_0(Word wt_i[WT_WORDS],
                       Word kh_i[KH_WORDS],
                       Word dmem_i[DMEM_WORDS],
                       Word dmem_o[DMEM_O_WORDS],
                       const Address n_inputs,
                       const Address n_outputs,
                       const Address input_words,
                       const Address output_words,
                       const ap_uint<3> layer_mode,  // [0]='new layer', [2:1]='conv1,conv,dense,last'
                       const ap_uint<1> dmem_mode,   // 0 means dmem[0] is input
                       const ap_uint<2> width_mode,  // 0=8'b, 1=16'b, 2=32'b
                       const ap_uint<2> norm_mode    // 0='do nothing', 1='do norm', 2='do pool'
) {
    image_cls(wt_i, kh_i, dmem_i, dmem_o, n_inputs, n_outputs, input_words, output_words, layer_mode, dmem_mode, width_mode, norm_mode);
    return 0;
}