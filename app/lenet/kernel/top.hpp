#ifndef __TOP_H__
#define __TOP_H__

#include <hls_stream.h>
#include <hls_task.h>

#include "conv1.hpp"
#include "samp2.hpp"
#include "conv3.hpp"
#include "samp4.hpp"
#include "conv5.hpp"
#include "full6.hpp"
#include "out.hpp"

void top(
    hls::stream<float>& in_stream,
    hls::stream<float>& conv1_weight_stream,
    hls::stream<float>& conv1_bias_stream,
    hls::stream<float>& conv3_weight_stream,
    hls::stream<float>& conv3_bias_stream,
    hls::stream<float>& conv5_weight_stream,
    hls::stream<float>& conv5_bias_stream,
    hls::stream<float>& full6_weight_stream,
    hls::stream<float>& full6_bias_stream,
    hls::stream<float>& out_weight_stream,
    hls::stream<float>& out_bias_stream,
    hls::stream<float>& out_stream
);

#endif