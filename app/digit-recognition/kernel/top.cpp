#include "top.hpp"

int digit_rec_kernel_0(WholeDigitType global_training_set[NUM_TRAINING],
                       WholeDigitType global_test_set[NUM_TEST],
                       LabelType global_results[NUM_TEST]) {
    digit_rec(global_training_set, global_test_set, global_results);
}