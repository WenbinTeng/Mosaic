#ifndef __TOP_IN_H__
#define __TOP_IN_H__

#include "typedef.h"

#include "hls_stream.h"

namespace top_in_space {

void top_in(
    whole_digit_t global_training_set[NUM_TRAINING],
    whole_digit_t global_test_set[NUM_TEST],
    hls::stream<whole_digit_t> &training_stream,
    hls::stream<whole_digit_t> &test_stream
);

}  // namespace top_in_space

#endif