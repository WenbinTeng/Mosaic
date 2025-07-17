#include "knn.hpp"

namespace knn_space {

void _init_training_set(
    hls::stream<digit_t> &training_stream, 
    whole_digit_t local_training_set[NUM_TRAINING]
) {
#pragma HLS INLINE
    for (int i = 0; i < NUM_TRAINING; i++) {
#pragma HLS PIPELINE
        local_training_set[i] = training_stream.read();
    }
}

void _init_knn_set(int knn_set[PAR_FACTOR * K_CONST]) {
#pragma HLS INLINE
    for (int i = 0; i < PAR_FACTOR * K_CONST; i++) {
#pragma HLS UNROLL
        knn_set[i] = 256;
    }
}

// popcount function
int _popcount(whole_digit_t x) {
#pragma HLS INLINE
    // most straightforward implementation
    // actually not bad on FPGA
    int cnt = 0;
    for (int i = 0; i < 256; i++)
#pragma HLS UNROLL
        cnt = cnt + x[i];

    return cnt;
}

// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.
void _update_knn(
    whole_digit_t test_inst,
    whole_digit_t train_inst,
    int min_distances[K_CONST]
) {
#pragma HLS INLINE

    // Compute the difference using XOR
    whole_digit_t diff = test_inst ^ train_inst;

    int dist = 0;
    dist = popcount(diff);

    int max_dist = 0;
    int max_dist_id = K_CONST + 1;
    int k = 0;

    // Find the max distance
    FIND_MAX_DIST:for (int k = 0; k < K_CONST; k++) {
        if (min_distances[k] > max_dist) {
            max_dist = min_distances[k];
            max_dist_id = k;
        }
    }

    // Replace the entry with the max distance
    if (dist < max_dist)
        min_distances[max_dist_id] = dist;

    return;
}

// Given 10xK minimum distance values, this function
// finds the actual K nearest neighbors and determines the
// final output based on the most common int represented by
// these nearest neighbors (i.e., a vote among KNNs).
label_t _knn_vote(int knn_set[PAR_FACTOR * K_CONST]) {
#pragma HLS INLINE

    // local buffers
    // final K nearest neighbors
    int min_distance_list[K_CONST];
#pragma HLS ARRAY_PARTITION variable=min_distance_list complete dim=0
    // labels for the K nearest neighbors
    int label_list[K_CONST];
#pragma HLS ARRAY_PARTITION variable=label_list complete dim=0
    // voting boxes
    int vote_list[10];
#pragma HLS ARRAY_PARTITION variable=vote_list complete dim=0

    int pos = 1000;

    // initialize
    for (int i = 0; i < K_CONST; i++) {
#pragma HLS UNROLL
        min_distance_list[i] = 256;
        label_list[i] = 9;
    }
    for (int i = 0; i < 10; i++) {
#pragma HLS UNROLL
        vote_list[i] = 0;
    }

    // go through all the lanes
    // do an insertion sort to keep a sorted neighbor list
    for (int i = 0; i < PAR_FACTOR; i++) {
        for (int j = 0; j < K_CONST; j++) {
#pragma HLS PIPELINE
            pos = 1000;
            for (int r = 0; r < K_CONST; r++) {
#pragma HLS UNROLL
                pos = ((knn_set[i * K_CONST + j] < min_distance_list[r]) && (pos > K_CONST)) ? r : pos;
            }
            for (int r = K_CONST; r > 0; r--) {
#pragma HLS UNROLL
                if (r - 1 > pos) {
                    min_distance_list[r - 1] = min_distance_list[r - 2];
                    label_list[r - 1] = label_list[r - 2];
                } else if (r - 1 == pos) {
                    min_distance_list[r - 1] = knn_set[i * K_CONST + j];
                    label_list[r - 1] = i / (PAR_FACTOR / 10);
                }
            }
        }
    }

    // vote
    for (int i = 0; i < K_CONST; i++) {
#pragma HLS PIPELINE
        vote_list[label_list[i]] += 1;
    }

    // find the maximum value
    label_t max_vote = 0;
    for (int i = 0; i < 10; i++) {
#pragma HLS UNROLL
        if (vote_list[i] >= vote_list[max_vote]) {
            max_vote = i;
        }
    }

    return max_vote;
}

void knn(
    hls::stream<whole_digit_t> &training_stream,
    hls::stream<label_t> &test_stream,
    hls::stream<label_t> &result_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=training_stream
#pragma HLS INTERFACE axis port=test_stream
#pragma HLS INTERFACE axis port=result_stream

    /*** Local training data buffer. ***/
    whole_digit_t local_training_set[NUM_TRAINING];
#pragma HLS ARRAY_PARTITION variable=local_training_set block factor=PAR_FACTOR dim=0
    _init_training_set(training_stream, local_training_set);

    /*** Top KNN for each PE. ***/
    int knn_set[PAR_FACTOR * K_CONST];
#pragma HLS ARRAY_PARTITION variable=knn_set complete dim=0

    for (int i = 0; i < NUM_TEST; i++) {
        label_t test_inst = test_stream.read();
        _init_knn(knn_set);
        for (int j = 0; j < NUM_TRAINING; j++) {
#pragma HLS PIPELINE
            for (int k = 0; k < PAR_FACTOR; k++) {
#pragma HLS UNROLL
                whole_digit_t training_inst = local_training_set[j * NUM_TRAINING / PAR_FACTOR + i];
                _update_knn(test_inst, training_test, &knn_set[k * K_CONST]);
            }
        }
        label_t max_vote = knn_vote(knn_set);
        result_stream.write(max_vote);
    }
}

}  // namespace knn_space