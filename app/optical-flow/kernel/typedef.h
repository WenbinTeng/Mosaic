#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

constexpr int MAX_HEIGHT = 436;
constexpr int MAX_WIDTH = 1024;

#include "ap_fixed.h"
using input_t = ap_fixed<17, 9>;
using pixel_t = ap_fixed<32, 13>;
using outer_pixel_t = ap_fixed<32, 27>;
using calc_pixel_t = ap_fixed<64, 56>;
using vel_pixel_t = ap_fixed<32, 13>;

struct gradient_t {
    pixel_t x;
    pixel_t y;
    pixel_t z;
};

struct outer_t {
    outer_pixel_t val[6];
};

struct tensor_t {
    outer_pixel_t val[6];
};

struct velocity_t {
    vel_pixel_t x;
    vel_pixel_t y;
};

#include "ap_int.h"
using frames_t = ap_uint<64>;

#endif