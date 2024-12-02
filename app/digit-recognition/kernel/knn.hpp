#ifndef __KNN_H__
#define __KNN_H__

#include "hls_stream.h"

#include "typedef.h"

void digit_rec(WholeDigitType global_training_set[NUM_TRAINING],
               WholeDigitType global_test_set[NUM_TEST],
               LabelType global_results[NUM_TEST]);

#endif