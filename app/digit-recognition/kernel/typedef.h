#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

// dataset information
constexpr int NUM_TRAINING = 18000;
constexpr int CLASS_SIZE = 1800;
constexpr int NUM_TEST = 2000;
constexpr int DIGIT_WIDTH = 4;
constexpr int NUM_LANE = 4;

// typedefs
using digit_t = unsigned long long;
using label_t = unsigned char;

#include "ap_int.h"
// sdsoc wide vector type
using whole_digit_t = ap_uint<256>

// parameters
constexpr int K_CONST = 3;
constexpr int PAR_FACTOR = 10;

#endif
