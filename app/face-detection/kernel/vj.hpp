#ifndef __VJ_H__
#define __VJ_H__

#include "typedef.h"

#include "hls_stream.h"

namespace vj_space {

void vj(
    hls::stream<data_t> &image_stream,
    hls::stream<result_t> &result_stream
);

}  // namespace vj_space

#endif