#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

// constants
constexpr int IMAGE_HEIGHT = 240;
constexpr int IMAGE_WIDTH = 320;
constexpr int RESULT_SIZE = 100;
constexpr int IMAGE_MAXGREY = 255;
constexpr int IMAGE_SIZE = (IMAGE_HEIGHT * IMAGE_WIDTH);
constexpr int TOTAL_NODES = 2913;
constexpr int TOTAL_STAGES = 25;
constexpr int TOTAL_COORDINATES = TOTAL_NODES * 12;
constexpr int TOTAL_WEIGHTS = TOTAL_NODES * 3;
constexpr int WINDOW_SIZE = 25;
constexpr int SQ_SIZE = 2;
constexpr int PYRAMID_HEIGHT = 12;
constexpr int ROWS = 25;
constexpr int COLS = 25;
constexpr int NUM_BANKS = 12;
constexpr int SIZE = 2913;

#include "ap_int.h"
// HLS-specific datatypes
using uint18_t = ap_uint<18>;
using uint10_t = ap_uint<10>;
using uint5_t = ap_int<5>;
using bit = ap_uint<1>;
using int_I = ap_uint<13>;
using int_SI = ap_uint<21>;
using int_II = ap_uint<18>;
using int_SII = ap_uint<26>;
using data_t = unsigned char;

// standard datatypes
struct point_t {
    int x;
    int y;
};

struct window_t {
    int width;
    int height;
};

struct rect_t {
    int x;
    int y;
    int width;
    int height;
};

struct result_t {
    rect_t r;
    int result;
};

struct image_t {
    int width;
    int height;
    int maxgrey;
    int flag;
};

#endif