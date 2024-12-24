#ifndef __BNN_H__
#define __BNN_H__

#include "hls_stream.h"

#include "typedef.h"

#pragma SDS data copy(dmem_i[0:input_words], dmem_o[0:output_words])
#pragma SDS data access_pattern(dmem_i:SEQUENTIAL, dmem_o:SEQUENTIAL)
#pragma SDS data access_pattern(wt_i:SEQUENTIAL, kh_i:SEQUENTIAL)
#pragma SDS data mem_attribute(dmem_i:PHYSICAL_CONTIGUOUS, dmem_o:PHYSICAL_CONTIGUOUS)
#pragma SDS data mem_attribute(wt_i:PHYSICAL_CONTIGUOUS, kh_i:PHYSICAL_CONTIGUOUS)
#pragma SDS data data_mover(dmem_i:AXIDMA_SIMPLE, dmem_o:AXIDMA_SIMPLE)
#pragma SDS data data_mover(wt_i:AXIDMA_SIMPLE, kh_i:AXIDMA_SIMPLE)
void image_cls(Word wt_i[WT_WORDS],
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
);

#endif