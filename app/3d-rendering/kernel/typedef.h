#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

// resolution 256x256
constexpr int MAX_X = 256;
constexpr int MAX_Y = 256;

// number of values in frame buffer: 32 bits
constexpr int NUM_FB = MAX_X * MAX_Y / 4;
// dataset information
constexpr int NUM_3D_TRI = 3192;

// hls header
#include "ap_int.h"
// specialized datatypes
using bit1 = ap_uint<1>;
using bit2 = ap_uint<2>;
using bit8 = ap_uint<8>;
using bit16 = ap_uint<16>;
using bit32 = ap_uint<32>;

// struct: 3D triangle
struct triangle_3d_t {
    bit8 x0;
    bit8 y0;
    bit8 z0;
    bit8 x1;
    bit8 y1;
    bit8 z1;
    bit8 x2;
    bit8 y2;
    bit8 z2;
};

// struct: 2D triangle
struct triangle_2d_t {
    bit8 x0;
    bit8 y0;
    bit8 x1;
    bit8 y1;
    bit8 x2;
    bit8 y2;
    bit8 z;
};

// struct: triangle info
struct triangle_info_t {
    bit2 flag;
    bit8 minX;
    bit8 maxX;
    bit8 minY;
    bit8 maxY;
    bit8 width;
    bit16 box;
};

// struct: candidate pixels
struct fragment_t {
    bit8 x;
    bit8 y;
    bit8 z;
    bit8 color;
};

// struct: colored pixel
struct pixel_t {
    bit8 x;
    bit8 y;
    bit8 color;
};

#endif