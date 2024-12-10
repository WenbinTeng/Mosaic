#include "top.hpp"

int digit_rec_kernel_0(WholeDigitType global_training_set[NUM_TRAINING],
                       WholeDigitType global_test_set[NUM_TEST],
                       LabelType global_results[NUM_TEST]) {

#pragma HLS INTERFACE s_axilite register port=return
#pragma HLS INTERFACE m_axi depth=NUM_TRAINING port=global_training_set offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_TEST port=global_test_set offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=NUM_TEST port=global_results offset=slave bundle=data

    digit_rec(global_training_set, global_test_set, global_results);
}