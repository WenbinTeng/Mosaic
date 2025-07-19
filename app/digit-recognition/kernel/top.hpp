#ifndef __TOP_H__
#define __TOP_H__

#include "typedef.h"

#include "hls_stream.h"
#include "hls_task.h"

#include "knn.hpp"
#include "top_in.hpp"
#include "top_out.hpp"

void digit_rec_kernel_0(
    whole_digit_t global_training_set[NUM_TRAINING],
    whole_digit_t global_test_set[NUM_TEST],
    label_t global_results[NUM_TEST]
);

#endif