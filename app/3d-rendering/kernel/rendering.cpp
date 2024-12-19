/*===============================================================*/
/*                                                               */
/*                        rendering.cpp                          */
/*                                                               */
/*                 C++ kernel for 3D Rendering                   */
/*                                                               */
/*===============================================================*/

#include "rendering.hpp"

/*======================UTILITY FUNCTIONS========================*/

// Determine whether three vertices of a trianlgLe
// (x0,y0) (x1,y1) (x2,y2) are in clockwise order by Pineda algorithm
// if so, return a number > 0
// else if three points are in line, return a number == 0
// else in counterclockwise order, return a number < 0
int check_clockwise(Triangle2D triangle_2d) {
    int cw;

    cw = (triangle_2d.x2 - triangle_2d.x0) * (triangle_2d.y1 - triangle_2d.y0) -
         (triangle_2d.y2 - triangle_2d.y0) * (triangle_2d.x1 - triangle_2d.x0);

    return cw;
}

// swap (x0, y0) (x1, y1) of a Triangle_2D
void clockwise_vertices(Triangle2D *triangle_2d) {

    bit8 tmp_x, tmp_y;

    tmp_x = triangle_2d->x0;
    tmp_y = triangle_2d->y0;

    triangle_2d->x0 = triangle_2d->x1;
    triangle_2d->y0 = triangle_2d->y1;

    triangle_2d->x1 = tmp_x;
    triangle_2d->y1 = tmp_y;
}

// Given a pixel, determine whether it is inside the triangle
// by Pineda algorithm
// if so, return true
// else, return false
bit1 pixel_in_triangle(bit8 x, bit8 y, Triangle2D triangle_2d) {

    int pi0, pi1, pi2;

    pi0 = (x - triangle_2d.x0) * (triangle_2d.y1 - triangle_2d.y0) -
          (y - triangle_2d.y0) * (triangle_2d.x1 - triangle_2d.x0);
    pi1 = (x - triangle_2d.x1) * (triangle_2d.y2 - triangle_2d.y1) -
          (y - triangle_2d.y1) * (triangle_2d.x2 - triangle_2d.x1);
    pi2 = (x - triangle_2d.x2) * (triangle_2d.y0 - triangle_2d.y2) -
          (y - triangle_2d.y2) * (triangle_2d.x0 - triangle_2d.x2);

    return (pi0 >= 0 && pi1 >= 0 && pi2 >= 0);
}

// find the min from 3 integers
bit8 find_min(bit8 in0, bit8 in1, bit8 in2) {
    if (in0 < in1) {
        if (in0 < in2)
            return in0;
        else
            return in2;
    } else {
        if (in1 < in2)
            return in1;
        else
            return in2;
    }
}

// find the max from 3 integers
bit8 find_max(bit8 in0, bit8 in1, bit8 in2) {
    if (in0 > in1) {
        if (in0 > in2)
            return in0;
        else
            return in2;
    } else {
        if (in1 > in2)
            return in1;
        else
            return in2;
    }
}

/*======================PROCESSING STAGES========================*/

void read_input(bit32 input[3 * NUM_3D_TRI], bit16 index, hls::stream<Triangle3D> &triangle_3d_stream) {
#pragma HLS inline

    Triangle3D triangle_3d;

    bit32 input_lo = input[3 * index];
    bit32 input_mi = input[3 * index + 1];
    bit32 input_hi = input[3 * index + 2];

    triangle_3d.x0 = input_lo( 7,  0);
    triangle_3d.y0 = input_lo(15,  8);
    triangle_3d.z0 = input_lo(23, 16);
    triangle_3d.x1 = input_lo(31, 24);
    triangle_3d.y1 = input_mi( 7,  0);
    triangle_3d.z1 = input_mi(15,  8);
    triangle_3d.x2 = input_mi(23, 16);
    triangle_3d.y2 = input_mi(31, 24);
    triangle_3d.z2 = input_hi( 7,  0);

    triangle_3d_stream.write(triangle_3d);
}

void projection(hls::stream<Triangle3D> &triangle_3d_stream,
                hls::stream<Triangle2D> &triangle_2d_stream) {

    Triangle3D triangle_3d = triangle_3d_stream.read();
    Triangle2D triangle_2d;

    triangle_2d.x0 = triangle_3d.x0;
    triangle_2d.y0 = triangle_3d.y0;
    triangle_2d.x1 = triangle_3d.x1;
    triangle_2d.y1 = triangle_3d.y1;
    triangle_2d.x2 = triangle_3d.x2;
    triangle_2d.y2 = triangle_3d.y2;
    triangle_2d.z = triangle_3d.z0 / 3 + triangle_3d.z1 / 3 + triangle_3d.z2 / 3;

    triangle_2d_stream.write(triangle_2d);
}

void rasterization1(hls::stream<Triangle2D> &triangle_2d_stream,
                    hls::stream<Triangle2D> &triangle_2d_same_stream,
                    hls::stream<TriangleInfo> &triangle_info_stream) {

    Triangle2D triangle_2d = triangle_2d_stream.read();
    Triangle2D triangle_2d_same;
    TriangleInfo triangle_info;

    if (check_clockwise(triangle_2d) == 0)
        return;
    if (check_clockwise(triangle_2d) < 0)
        clockwise_vertices(&triangle_2d);

    triangle_2d_same.x0 = triangle_2d.x0;
    triangle_2d_same.y0 = triangle_2d.y0;
    triangle_2d_same.x1 = triangle_2d.x1;
    triangle_2d_same.y1 = triangle_2d.y1;
    triangle_2d_same.x2 = triangle_2d.x2;
    triangle_2d_same.y2 = triangle_2d.y2;
    triangle_2d_same.z = triangle_2d.z;

    triangle_info.minX = find_min(triangle_2d.x0, triangle_2d.x1, triangle_2d.x2);
    triangle_info.maxX = find_max(triangle_2d.x0, triangle_2d.x1, triangle_2d.x2);
    triangle_info.minY = find_min(triangle_2d.y0, triangle_2d.y1, triangle_2d.y2);
    triangle_info.maxY = find_max(triangle_2d.y0, triangle_2d.y1, triangle_2d.y2);
    triangle_info.width = triangle_info.maxX - triangle_info.minX;
    triangle_info.box = (triangle_info.maxX - triangle_info.minX) * (triangle_info.maxY - triangle_info.minY);
    triangle_info.flag = 0;

    triangle_2d_same_stream.write(triangle_2d_same);
    triangle_info_stream.write(triangle_info);
}

void rasterization2(hls::stream<Triangle2D> &triangle_2d_same_stream,
                    hls::stream<TriangleInfo> &triangle_info_stream,
                    hls::stream<Fragment> &fragment_stream) {

    Triangle2D triangle_2d_same = triangle_2d_same_stream.read();
    TriangleInfo triangle_info = triangle_info_stream.read();

    bit8 color = 100;

    for (bit16 k = 0; k < triangle_info.box; k++) {
#pragma HLS pipeline II = 1
        Fragment fragment;
        bit8 x = triangle_info.minX + k % triangle_info.width;
        bit8 y = triangle_info.minY + k / triangle_info.width;

        if (pixel_in_triangle(x, y, triangle_2d_same)) {
            fragment.x = x;
            fragment.y = y;
            fragment.z = triangle_2d_same.z;
            fragment.color = color;
            fragment_stream.write(fragment);
        }
    }
}

void zculling(bit16 index,
              hls::stream<Fragment> &fragment_stream,
              hls::stream<Pixel> &pixel_stream) {
    
    static bit8 z_buffer[MAX_X][MAX_Y];
    if (index == 0) {
        ZCULLING_INIT_ROW:for (bit16 i = 0; i < MAX_X; i++) {
#pragma HLS PIPELINE II = 1
            ZCULLING_INIT_COL:for (bit16 j = 0; j < MAX_Y; j++) {
                z_buffer[i][j] = 255;
            }
        }
    }

    Fragment fragment = fragment_stream.read();

    if (fragment.z < z_buffer[fragment.x][fragment.y]) {
        Pixel pixel;
        pixel.x = fragment.x;
        pixel.y = fragment.y;
        pixel.color = fragment.color;
        z_buffer[fragment.x][fragment.y] = fragment.z;
        pixel_stream.write(pixel);
    }
}

void coloring(bit16 index,
              hls::stream<Pixel> &pixel_stream,
              bit8 frame_buffer[MAX_X][MAX_Y]) {
    
    if (index == 0) {
        COLORING_INIT_ROW:for (bit16 i = 0; i < MAX_X; i++) {
#pragma HLS PIPELINE II = 1
            COLORING_INIT_COL:for (bit16 j = 0; j < MAX_Y; j++) {
                frame_buffer[i][j] = 0;
            }
        }
    }

    Pixel pixel = pixel_stream.read();
    frame_buffer[pixel.x][pixel.y] = pixel.color;
}

void write_output(bit8 frame_buffer[MAX_X][MAX_Y], bit32 output[NUM_FB]) {
#pragma HLS inline

    bit32 out_tmp = 0;
    OUTPUT_FB_ROW:for (bit16 i = 0; i < MAX_X; i++) {
#pragma HLS PIPELINE II = 1
        OUTPUT_FB_COL:for (bit16 j = 0; j < MAX_Y; j = j + 4) {
            out_tmp( 7,  0) = frame_buffer[i][j + 0];
            out_tmp(15,  8) = frame_buffer[i][j + 1];
            out_tmp(23, 16) = frame_buffer[i][j + 2];
            out_tmp(31, 24) = frame_buffer[i][j + 3];
            output[i * MAX_Y / 4 + j / 4] = out_tmp;
        }
    }
}

/*========================TOP FUNCTION===========================*/

void rendering(bit32 input[3 * NUM_3D_TRI], bit32 output[NUM_FB]) {
#pragma HLS dataflow
    
    bit8 frame_buffer[MAX_X][MAX_Y];
    hls::stream<Triangle3D> triangle_3d_stream;
    hls::stream<Triangle2D> triangle_2d_stream;
    hls::stream<Triangle2D> triangle_2d_same_stream;
    hls::stream<TriangleInfo> triangle_info_stream;
    hls::stream<Fragment> fragment_stream;
    hls::stream<Pixel> pixel_stream;

    TRIANGLES:for (bit16 i = 0; i < NUM_3D_TRI; i++) {
        read_input(input, i, triangle_3d_stream);
        projection(triangle_3d_stream, triangle_2d_stream);
        rasterization1(triangle_2d_stream, triangle_2d_same_stream, triangle_info_stream);
        rasterization2(triangle_2d_same_stream, triangle_info_stream, fragment_stream);
        zculling(i, fragment_stream, pixel_stream);
        coloring(i, pixel_stream, frame_buffer);
    }

    write_output(frame_buffer, output);
}
