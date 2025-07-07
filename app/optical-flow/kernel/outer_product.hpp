#ifndef __OUTER_PRODUCT_H__
#define __OUTER_PRODUCT_H__

#include "typedef.h"

#include "hls_stream.h"

namespace outer_product_space {

void outer_product(
    hls::stream<gradient_t> &gradient_stream,
    hls::stream<outer_t> &outer_product_stream
);

}  // namespace outer_product_space

#endif