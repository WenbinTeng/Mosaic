#ifndef __GRAD_Z_CALC_H__
#define __GRAD_Z_CALC_H__

#include "typedef.h"

#include "hls_stream.h"

namespace grad_z_calc_space {

void grad_z_calc(
    hls::stream<input_t> &frame1_stream,
    hls::stream<input_t> &frame2_stream,
    hls::stream<input_t> &frame3_stream,
    hls::stream<input_t> &frame4_stream,
    hls::stream<input_t> &frame5_stream,
    hls::stream<pixel_t> &gradient_z_stream
);

}  // namespace grad_z_calc_space

#endif