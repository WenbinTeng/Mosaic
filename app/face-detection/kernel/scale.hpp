#ifndef __SCALE_H__
#define __SCALE_H__

#include "typedef.h"

#include "hls_stream.h"

namespace scale_space {

void scale(
    hls::stream<data_t> &in_stream,
    hls::stream<data_t> &out_stream
);

}  // namespace scale_space

#endif