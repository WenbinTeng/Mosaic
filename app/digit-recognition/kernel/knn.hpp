#ifndef __KNN_H__
#define __KNN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace knn_space {

void knn(
    hls::stream<whole_digit_t> &training_stream,
    hls::stream<whole_digit_t> &test_stream,
    hls::stream<label_t> &result_stream
);

}  // namespace knn_space

#endif