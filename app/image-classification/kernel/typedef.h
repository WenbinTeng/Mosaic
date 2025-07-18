#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

constexpr unsigned CONVOLVERS = 2;

constexpr unsigned WORD_SIZE = 64;
constexpr unsigned WT_SIZE = 9;
constexpr unsigned CONV_W_PER_WORD = 7;
constexpr unsigned CONV1_W_PER_WORD = 4;
constexpr unsigned KH_PER_WORD = 4;
constexpr unsigned BYTE_SIZE = 8;
constexpr unsigned K = 3;
constexpr unsigned WT_L = 16 * 4 * 512;  // parameter to control wt mem size
constexpr unsigned C_WT_WORDS = ((WT_L + CONV_W_PER_WORD - 1) / CONV_W_PER_WORD + CONVOLVERS - 1) / CONVOLVERS;  // wt words per convolver
constexpr unsigned WT_WORDS = C_WT_WORDS * CONVOLVERS;
constexpr unsigned KH_WORDS = WT_L / 128 * 16 / WORD_SIZE;

constexpr unsigned DMEM_WORDS = 128 * 32 * 32 / WORD_SIZE;
constexpr unsigned C_DMEM_WORDS = DMEM_WORDS / CONVOLVERS;
constexpr unsigned DMEM_O_WORDS = 512 * 4 * 4 / WORD_SIZE;
constexpr unsigned DB_MEM_WORDS = 32 * 32;

constexpr unsigned PIX_PER_PHASE = 2 * 32 * 32;

constexpr unsigned MAX_WIDTH = WORD_SIZE;
constexpr unsigned BANK_WIDTH = 8;
constexpr unsigned LOG_BANK_WIDTH = 3;

constexpr unsigned CONV_ROWS = 3;
constexpr unsigned CONV_COLS = BANK_WIDTH + 2;
constexpr unsigned CONV_BANKS = WORD_SIZE / BANK_WIDTH;

enum LayerTypeEnum { LAYER_CONV1, LAYER_CONV, LAYER_DENSE, LAYER_LAST };

#include "ap_int.h"
typedef ap_int<WORD_SIZE> Word;
typedef ap_int<WT_SIZE> WtType;
typedef ap_uint<16> Address;
typedef ap_int<12> ConvSum;
typedef ap_int<5> ConvOut;
typedef ap_uint<10> IdxType;
typedef ap_fixed<16, 4> C1Comp;
typedef ap_int<16> NormComp;
typedef ap_int<16> DenseSum;
typedef ap_fixed<16, 12> DenseNorm;

typedef ap_fixed<20, 2, AP_RND> C1InputType;
typedef ap_fixed<24, 6, AP_RND> C1ConvType;

// Quantized 32-bit input images in the range [-1,1]
typedef ap_fixed<32, 2, AP_RND> InputFixed;

// Types for weights
typedef ap_int<1> Bit;
typedef ap_int<2> TwoBit;

typedef ap_fixed<16, 2> KType;
typedef ap_fixed<16, 4> HType;

typedef ap_fixed<16, 5> NormOutput;
typedef ap_int<14> ConvOutput;

#endif