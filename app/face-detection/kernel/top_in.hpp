#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_in_space {

void top_in(
    data_t data[IMAGE_WIDTH],
    hls::stream<data_t> &pixel_stream
);

}  // namespace top_in_space

#endif