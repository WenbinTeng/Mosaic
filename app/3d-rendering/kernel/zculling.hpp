#ifndef __ZCULLING_H__
#define __ZCULLING_H__

#include "typedef.h"

#include "hls_stream.h"

namespace zculling_space {

void zculling(
    hls::stream<fragment_t> &fragment_stream,
    hls::stream<pixel_t> &pixel_stream
);

}  // namespace zculling_space

#endif