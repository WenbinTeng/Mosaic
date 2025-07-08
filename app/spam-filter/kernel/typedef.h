#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

// dataset information
constexpr int NUM_FEATURES  = 1024;
constexpr int NUM_SAMPLES   = 5000;
constexpr int NUM_TRAINING  = 4500;
constexpr int NUM_TESTING   = 500;
constexpr int STEP_SIZE     = 60000; 
constexpr int NUM_EPOCHS    = 5;
constexpr int DATA_SET_SIZE = NUM_FEATURES * NUM_SAMPLES;

// datatypes for accelerator

// embedded platforms have less off-chip bandwidth
constexpr int VFTYPE_WIDTH = 64;
constexpr int VDTYPE_WIDTH = 64;
constexpr int VLTYPE_WIDTH = 32;

#include "ap_fixed.h"
#include "ap_int.h"
// features / parameters
constexpr int FTYPE_TWIDTH = 32;
constexpr int FTYPE_IWIDTH = 32;
using feature_t = ap_fixed<FTYPE_TWIDTH, FTYPE_IWIDTH>;
using vec_feature_t = ap_uint<VFTYPE_WIDTH>;
constexpr unsigned int F_VECTOR_SIZE = VFTYPE_WIDTH / FTYPE_TWIDTH;
// training data
constexpr int DTYPE_TWIDTH = 16;
constexpr int DTYPE_IWIDTH = 4;
using data_t = ap_fixed<DTYPE_TWIDTH, DTYPE_IWIDTH>;
using vec_data_t = ap_uint<VDTYPE_WIDTH>;
constexpr unsigned int D_VECTOR_SIZE = VDTYPE_WIDTH / DTYPE_TWIDTH;
// label
constexpr int LTYPE_WIDTH = 8;
using label_t = ap_uint<LTYPE_WIDTH>;
using vec_label_t = ap_uint<VLTYPE_WIDTH>;
constexpr unsigned int L_VECTOR_SIZE = VLTYPE_WIDTH / LTYPE_WIDTH;

// datatypes for the LUT
constexpr int LUTOUT_TWIDTH = 12;
constexpr int LUTOUT_IWIDTH = 2;
constexpr int LUTIN_TWIDTH = 12;
constexpr int LUTIN_IWIDTH = 4;
using tmp_fixed_t = ap_ufixed<32, 20>;
using idx_fixed_t = ap_uint<LUTIN_TWIDTH>;
using lutin_fixed_t = ap_fixed<LUTIN_TWIDTH, LUTIN_IWIDTH>;
using lutout_fixed_t = ap_fixed<LUTOUT_TWIDTH, LUTOUT_IWIDTH>;

constexpr int PAR_FACTOR = 32;

#endif