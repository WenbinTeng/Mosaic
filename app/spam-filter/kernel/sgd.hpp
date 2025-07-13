#ifndef __SGD_H__
#define __SGD_H__

#include "typedef.h"
#include "lut.h"

#include "hls_stream.h"

namespace sgd_space {

void sgd(
    hls::stream<data_t> &data_stream,
    hls::stream<label_t> &label_stream,
    hls::stream<feature_t> &theta_stream
);

}  // namespace sgd_space

#endif