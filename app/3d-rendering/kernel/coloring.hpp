#ifndef __COLORING_H__
#define __COLORING_H__

#include "typedef.h"

#include "hls_stream.h"

namespace coloring_space {

void coloring(
    hls::stream<pixel_t> &pixel_stream,
    hls::stream<bit8> &frame_stream
);

}  // namespace coloring_space

#endif