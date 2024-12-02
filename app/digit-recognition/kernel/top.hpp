#ifndef __TOP_H__
#define __TOP_H__

#include "knn.hpp"

int digit_rec_kernel_0(WholeDigitType global_training_set[NUM_TRAINING],
                       WholeDigitType global_test_set[NUM_TEST],
                       LabelType global_results[NUM_TEST]);

#endif