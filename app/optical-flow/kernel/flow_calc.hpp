#ifndef __FLOW_CALC_H__
#define __FLOW_CALC_H__

#include "typedef.h"

#include "hls_stream.h"

namespace flow_calc_space {

void flow_calc(
    hls::stream<tensor_t> &tensor_stream,
    hls::stream<velocity_t> &output_stream
);

}  // namespace flow_calc_space

#endif