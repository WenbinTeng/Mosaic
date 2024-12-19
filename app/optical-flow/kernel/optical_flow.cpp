/*===============================================================*/
/*                                                               */
/*                      optical_flow.cpp                         */
/*                                                               */
/*             Hardware function for optical flow                */
/*                                                               */
/*===============================================================*/

#include "optical_flow.hpp"

// define these constants so they can be used in pragma
const int max_width = MAX_WIDTH;
const int default_depth = MAX_WIDTH;

void gradient_xy_calc(hls::stream<input_t> &frame_stream,
                      hls::stream<pixel_t> &gradient_x_stream,
                      hls::stream<pixel_t> &gradient_y_stream) {
    // our own line buffer
    static pixel_t buf[5][MAX_WIDTH];
#pragma HLS array_partition variable = buf complete dim = 1

    // small buffer
    pixel_t small_buf[5];
#pragma HLS array_partition variable = small_buf complete dim = 0

    // window buffer
    xf::cv::Window<5, 5, input_t> window;

    const int GRAD_WEIGHTS[] = {1, -8, 0, 8, -1};

    GRAD_XY_OUTER:for (int r = 0; r < MAX_HEIGHT + 2; r++) {
        GRAD_XY_INNER:for (int c = 0; c < MAX_WIDTH + 2; c++) {
#pragma HLS pipeline II = 1
            // read out values from current line buffer
            for (int i = 0; i < 4; i++)
                small_buf[i] = buf[i + 1][c];
            // the new value is either 0 or read from frame
            if (r < MAX_HEIGHT && c < MAX_WIDTH)
                small_buf[4] = (pixel_t)frame_stream.read();
            else if (c < MAX_WIDTH)
                small_buf[4] = 0;
            // update line buffer
            if (r < MAX_HEIGHT && c < MAX_WIDTH) {
                for (int i = 0; i < 4; i++)
                    buf[i][c] = small_buf[i];
                buf[4][c] = small_buf[4];
            } else if (c < MAX_WIDTH) {
                for (int i = 0; i < 4; i++)
                    buf[i][c] = small_buf[i];
                buf[4][c] = small_buf[4];
            }

            // manage window buffer
            if (r < MAX_HEIGHT && c < MAX_WIDTH) {
                window.shift_pixels_left();
                for (int i = 0; i < 5; i++)
                    window.insert_pixel(small_buf[i], i, 4);
            } else {
                window.shift_pixels_left();
                window.insert_pixel(0, 0, 4);
                window.insert_pixel(0, 1, 4);
                window.insert_pixel(0, 2, 4);
                window.insert_pixel(0, 3, 4);
                window.insert_pixel(0, 4, 4);
            }

            // compute gradient
            pixel_t x_grad = 0;
            pixel_t y_grad = 0;
            if (r >= 4 && r < MAX_HEIGHT && c >= 4 && c < MAX_WIDTH) {
                GRAD_XY_XYGRAD:for (int i = 0; i < 5; i++) {
                    x_grad += window.getval(2, i) * GRAD_WEIGHTS[i];
                    y_grad += window.getval(i, 2) * GRAD_WEIGHTS[i];
                }
                gradient_x_stream.write(x_grad / 12);
                gradient_y_stream.write(y_grad / 12);
            } else if (r >= 2 && c >= 2) {
                gradient_x_stream.write(0);
                gradient_y_stream.write(0);
            }
        }
    }
}

void gradient_z_calc(hls::stream<input_t> &frame1_stream,
                     hls::stream<input_t> &frame2_stream,
                     hls::stream<input_t> &frame3_stream,
                     hls::stream<input_t> &frame4_stream,
                     hls::stream<input_t> &frame5_stream,
                     hls::stream<pixel_t> &gradient_z_stream) {
    const int GRAD_WEIGHTS[] = {1, -8, 0, 8, -1};
    GRAD_Z_OUTER:for (int r = 0; r < MAX_HEIGHT; r++) {
        GRAD_Z_INNER:for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS pipeline II = 1
            input_t f1 = frame1_stream.read();
            input_t f2 = frame2_stream.read();
            input_t f3 = frame3_stream.read();
            input_t f4 = frame4_stream.read();
            input_t f5 = frame5_stream.read();
            gradient_z_stream.write(((pixel_t)(f1 * GRAD_WEIGHTS[0] +
                                               f2 * GRAD_WEIGHTS[1] +
                                               f3 * GRAD_WEIGHTS[2] +
                                               f4 * GRAD_WEIGHTS[3] +
                                               f5 * GRAD_WEIGHTS[4])) / 12);
        }
    }
}

void gradient_weight_y(hls::stream<pixel_t> &gradient_x_stream,
                       hls::stream<pixel_t> &gradient_y_stream,
                       hls::stream<pixel_t> &gradient_z_stream,
                       hls::stream<gradient_t> &y_filt_grad_stream) {
    xf::cv::LineBuffer<7, MAX_WIDTH, gradient_t> buf;
    const pixel_t GRAD_FILTER[] = {0.0755, 0.133, 0.1869, 0.2903, 0.1869, 0.133, 0.0755};
    GRAD_WEIGHT_Y_OUTER:for (int r = 0; r < MAX_HEIGHT + 3; r++) {
        GRAD_WEIGHT_Y_INNER:for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS pipeline II = 1
#pragma HLS dependence variable = buf inter false
            if (r < MAX_HEIGHT) {
                buf.shift_pixels_up(c);
                gradient_t tmp;
                tmp.x = gradient_x_stream.read();
                tmp.y = gradient_y_stream.read();
                tmp.z = gradient_z_stream.read();
                buf.insert_bottom_row(tmp, c);
            } else {
                buf.shift_pixels_up(c);
                gradient_t tmp;
                tmp.x = 0;
                tmp.y = 0;
                tmp.z = 0;
                buf.insert_bottom_row(tmp, c);
            }

            gradient_t acc;
            acc.x = 0;
            acc.y = 0;
            acc.z = 0;
            if (r >= 6 && r < MAX_HEIGHT) {
                GRAD_WEIGHT_Y_ACC:for (int i = 0; i < 7; i++) {
                    acc.x += buf.getval(i, c).x * GRAD_FILTER[i];
                    acc.y += buf.getval(i, c).y * GRAD_FILTER[i];
                    acc.z += buf.getval(i, c).z * GRAD_FILTER[i];
                }
                y_filt_grad_stream.write(acc);
            } else if (r >= 3) {
                y_filt_grad_stream.write(acc);
            }
        }
    }
}

void gradient_weight_x(hls::stream<gradient_t> &y_filt_grad_stream,
                       hls::stream<gradient_t> &filt_grad_stream) {
    xf::cv::Window<1, 7, gradient_t> buf;
    const pixel_t GRAD_FILTER[] = {0.0755, 0.133, 0.1869, 0.2903, 0.1869, 0.133, 0.0755};
    GRAD_WEIGHT_X_OUTER:for (int r = 0; r < MAX_HEIGHT; r++) {
        GRAD_WEIGHT_X_INNER:for (int c = 0; c < MAX_WIDTH + 3; c++) {
#pragma HLS pipeline II = 1
            buf.shift_pixels_left();
            gradient_t tmp;
            if (c < MAX_WIDTH) {
                tmp = y_filt_grad_stream.read();
            } else {
                tmp.x = 0;
                tmp.y = 0;
                tmp.z = 0;
            }
            buf.insert_pixel(tmp, 0, 6);

            gradient_t acc;
            acc.x = 0;
            acc.y = 0;
            acc.z = 0;
            if (c >= 6 && c < MAX_WIDTH) {
                GRAD_WEIGHT_X_ACC:for (int i = 0; i < 7; i++) {
                    acc.x += buf.getval(0, i).x * GRAD_FILTER[i];
                    acc.y += buf.getval(0, i).y * GRAD_FILTER[i];
                    acc.z += buf.getval(0, i).z * GRAD_FILTER[i];
                }
                filt_grad_stream.write(acc);
            } else if (c >= 3) {
                filt_grad_stream.write(acc);
            }
        }
    }
}

void outer_product(hls::stream<gradient_t> &gradient_stream,
                   hls::stream<outer_t> &outer_product_stream) {
    OUTER_OUTER:for (int r = 0; r < MAX_HEIGHT; r++) {
        OUTER_INNER:for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS pipeline II = 1
            gradient_t grad = gradient_stream.read();
            outer_pixel_t x = (outer_pixel_t)grad.x;
            outer_pixel_t y = (outer_pixel_t)grad.y;
            outer_pixel_t z = (outer_pixel_t)grad.z;
            outer_t out;
            out.val[0] = (x * x);
            out.val[1] = (y * y);
            out.val[2] = (z * z);
            out.val[3] = (x * y);
            out.val[4] = (x * z);
            out.val[5] = (y * z);
            outer_product_stream.write(out);
        }
    }
}

void tensor_weight_y(hls::stream<outer_t> &outer_product_stream,
                     hls::stream<tensor_t> &tensor_y_stream) {
    xf::cv::LineBuffer<3, MAX_WIDTH, outer_t> buf;
    const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};
    TENSOR_WEIGHT_Y_OUTER:for (int r = 0; r < MAX_HEIGHT + 1; r++) {
        TENSOR_WEIGHT_Y_INNER:for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS pipeline II = 1
            outer_t tmp;
            buf.shift_pixels_up(c);
            if (r < MAX_HEIGHT) {
                tmp = outer_product_stream.read();
            } else {
                TENSOR_WEIGHT_Y_TMP_INIT:for (int i = 0; i < 6; i++)
                    tmp.val[i] = 0;
            }
            buf.insert_bottom_row(tmp, c);

            tensor_t acc;
            TENSOR_WEIGHT_Y_ACC_INIT:for (int k = 0; k < 6; k++)
                acc.val[k] = 0;

            if (r >= 2 && r < MAX_HEIGHT) {
                TENSOR_WEIGHT_Y_TMP_OUTER:for (int i = 0; i < 3; i++) {
                    tmp = buf.getval(i, c);
                    pixel_t k = TENSOR_FILTER[i];
                    TENSOR_WEIGHT_Y_TMP_INNER:for (int component = 0; component < 6; component++) {
                        acc.val[component] += tmp.val[component] * k;
                    }
                }
            }
            if (r >= 1) {
                tensor_y_stream.write(acc);
            }
        }
    }
}

void tensor_weight_x(hls::stream<tensor_t> &tensor_y_stream,
                     hls::stream<tensor_t> &tensor_stream) {
    xf::cv::Window<1, 3, tensor_t> buf;
    const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};
    TENSOR_WEIGHT_X_OUTER:for (int r = 0; r < MAX_HEIGHT; r++) {
        TENSOR_WEIGHT_X_INNER:for (int c = 0; c < MAX_WIDTH + 1; c++) {
#pragma HLS pipeline II = 1
            buf.shift_pixels_left();
            tensor_t tmp;
            if (c < MAX_WIDTH) {
                tmp = tensor_y_stream.read();
            } else {
                TENSOR_WEIGHT_X_TMP_INIT:for (int i = 0; i < 6; i++)
                    tmp.val[i] = 0;
            }
            buf.insert_pixel(tmp, 0, 2);

            tensor_t acc;
            TENSOR_WEIGHT_X_ACC_INIT:for (int k = 0; k < 6; k++)
                acc.val[k] = 0;
            if (c >= 2 && c < MAX_WIDTH) {
                TENSOR_WEIGHT_X_TMP_OUTER:for (int i = 0; i < 3; i++) {
                    tmp = buf.getval(0, i);
                    TENSOR_WEIGHT_X_TMP_INNER:for (int component = 0; component < 6; component++) {
                        acc.val[component] += tmp.val[component] * TENSOR_FILTER[i];
                    }
                }
            }
            if (c >= 1) {
                tensor_stream.write(acc);
            }
        }
    }
}

void flow_calc(hls::stream<tensor_t> &tensor_stream,
               velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]) {
    static outer_pixel_t buf[2];
    FLOW_OUTER:for (int r = 0; r < MAX_HEIGHT; r++) {
        FLOW_INNER:for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS pipeline II = 1
            tensor_t tmp_tensor = tensor_stream.read();
            if (r >= 2 && r < MAX_HEIGHT - 2 && c >= 2 && c < MAX_WIDTH - 2) {
                calc_pixel_t t1 = (calc_pixel_t)tmp_tensor.val[0];
                calc_pixel_t t2 = (calc_pixel_t)tmp_tensor.val[1];
                calc_pixel_t t3 = (calc_pixel_t)tmp_tensor.val[2];
                calc_pixel_t t4 = (calc_pixel_t)tmp_tensor.val[3];
                calc_pixel_t t5 = (calc_pixel_t)tmp_tensor.val[4];
                calc_pixel_t t6 = (calc_pixel_t)tmp_tensor.val[5];

                calc_pixel_t denom = t1 * t2 - t4 * t4;
                calc_pixel_t numer0 = t6 * t4 - t5 * t2;
                calc_pixel_t numer1 = t5 * t4 - t6 * t1;

                if (denom != 0) {
                    buf[0] = numer0 / denom;
                    buf[1] = numer1 / denom;
                } else {
                    buf[0] = 0;
                    buf[1] = 0;
                }
            } else {
                buf[0] = buf[1] = 0;
            }

            outputs[r][c].x = (vel_pixel_t)buf[0];
            outputs[r][c].y = (vel_pixel_t)buf[1];
        }
    }
}

void optical_flow(frames_t frames[MAX_HEIGHT][MAX_WIDTH],
                  velocity_t outputs[MAX_HEIGHT][MAX_WIDTH]) {
#pragma HLS dataflow

    static hls::stream<input_t> frame1_stream;
    static hls::stream<input_t> frame2_stream;
    static hls::stream<input_t> frame3_stream_a;
    static hls::stream<input_t> frame3_stream_b;
    static hls::stream<input_t> frame4_stream;
    static hls::stream<input_t> frame5_stream;

    static hls::stream<pixel_t> gradient_x_stream;
    static hls::stream<pixel_t> gradient_y_stream;
    static hls::stream<pixel_t> gradient_z_stream;
    static hls::stream<gradient_t> y_filt_grad_stream;
    static hls::stream<gradient_t> filt_grad_stream;
    static hls::stream<outer_t> outer_stream;
    static hls::stream<tensor_t> tensor_y_stream;
    static hls::stream<tensor_t> tensor_stream;

    static frames_t buf;
    FRAMES_CP_OUTER:for (int r = 0; r < MAX_HEIGHT; r++) {
        FRAMES_CP_INNER:for (int c = 0; c < MAX_WIDTH; c++) {
#pragma HLS pipeline II = 1
            // one wide read
            buf = frames[r][c];
            // assign values to the FIFOs
            frame1_stream.write((input_t)(buf(7, 0)) >> 8);
            frame2_stream.write((input_t)(buf(15, 8)) >> 8);
            frame3_stream_a.write((input_t)(buf(23, 16)) >> 8);
            frame3_stream_b.write((input_t)(buf(23, 16)) >> 8);
            frame4_stream.write((input_t)(buf(31, 24)) >> 8);
            frame5_stream.write((input_t)(buf(39, 32)) >> 8);
        }
    }

    gradient_xy_calc(frame3_stream_a, gradient_x_stream, gradient_y_stream);
    gradient_z_calc(frame1_stream, frame2_stream, frame3_stream_b, frame4_stream, frame5_stream, gradient_z_stream);
    gradient_weight_y(gradient_x_stream, gradient_y_stream, gradient_z_stream, y_filt_grad_stream);
    gradient_weight_x(y_filt_grad_stream, filt_grad_stream);
    outer_product(filt_grad_stream, outer_stream);
    tensor_weight_y(outer_stream, tensor_y_stream);
    tensor_weight_x(tensor_y_stream, tensor_stream);
    flow_calc(tensor_stream, outputs);
}
