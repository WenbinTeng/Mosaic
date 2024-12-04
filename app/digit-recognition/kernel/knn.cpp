/*===============================================================*/
/*                                                               */
/*                          digitrec.cpp                         */
/*                                                               */
/*             Hardware function for digit recognition           */
/*                                                               */
/*===============================================================*/

#include "knn.hpp"

// popcount function
int popcount(WholeDigitType x) {
    // most straightforward implementation
    // actually not bad on FPGA
    int cnt = 0;
    for (int i = 0; i < 256; i++)
#pragma HLS unroll
        cnt = cnt + x[i];

    return cnt;
}

// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.
void update_knn(WholeDigitType test_inst,
                WholeDigitType train_inst,
                int min_distances[K_CONST]) {
#pragma HLS inline

    // Compute the difference using XOR
    WholeDigitType diff = test_inst ^ train_inst;

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

void read_training_data_block(WholeDigitType global_training_set[NUM_TRAINING],
                              WholeDigitType local_training_set[NUM_TRAINING / PAR_FACTOR],
                              int start) {
    for (int i = 0; i < NUM_TRAINING / PAR_FACTOR; i++) {
        local_training_set[i] = global_training_set[start * NUM_TRAINING / PAR_FACTOR + i];
    }
}

void read_test_data(WholeDigitType global_test_set[NUM_TEST],
                    hls::stream<WholeDigitType> &test_data_stream) {
    for (int i = 0; i < NUM_TEST; i++) {
        test_data_stream.write(global_test_set[i]);
    }
}

void write_result(hls::stream<LabelType> &result_stream,
                  LabelType global_results[NUM_TEST]) {
    for (int i = 0; i < NUM_TEST; i++) {
        global_results[i] = result_stream.read();
    }
}

void knn_vote_small(int knn_set[NUM_LANE * K_CONST],
                    int min_distance_list[K_CONST],
                    int label_list[K_CONST],
                    int label_in) {
#pragma HLS inline
#pragma HLS array_partition variable = knn_set complete dim = 0
// final K nearest neighbors
#pragma HLS array_partition variable = min_distance_list complete dim = 0
// labels for the K nearest neighbors
#pragma HLS array_partition variable = label_list complete dim = 0

    int pos = 1000;

    LANES:for (int i = 0; i < NUM_LANE; i++) {
        INSERTION_SORT_OUTER:for (int j = 0; j < K_CONST; j++) {
#pragma HLS pipeline
            pos = 1000;
            INSERTION_SORT_INNER:for (int r = 0; r < K_CONST; r++) {
#pragma HLS unroll
                pos = ((knn_set[i * K_CONST + j] < min_distance_list[r]) && (pos > K_CONST)) ? r : pos;
            }

            INSERT:for (int r = K_CONST; r > 0; r--) {
#pragma HLS unroll
                if (r - 1 > pos) {
                    min_distance_list[r - 1] = min_distance_list[r - 2];
                    label_list[r - 1] = label_list[r - 2];
                } else if (r - 1 == pos) {
                    min_distance_list[r - 1] = knn_set[i * K_CONST + j];
                    label_list[r - 1] = label_in;
                }
            }
        }
    }
}

void knn_vote_final(hls::stream<int> &knn_set_stream,
                    hls::stream<LabelType> &result_stream) {

    int min_distance_list[K_CONST];
#pragma HLS array_partition variable = min_distance_list complete dim = 0

    int label_list[K_CONST];
#pragma HLS array_partition variable = label_list complete dim = 0

    int vote_list[10];
#pragma HLS array_partition variable = vote_list complete dim = 0

    for (int t = 0; t < NUM_TEST; t++) {
        INIT_0:for (int i = 0; i < K_CONST; i++) {
            min_distance_list[i] = knn_set_stream.read();
        }
        INIT_1:for (int i = 0; i < K_CONST; i++) {
            label_list[i] = knn_set_stream.read();
        }
        INIT:for (int i = 0; i < 10; i++) {
    #pragma HLS unroll
            vote_list[i] = 0;
        }

        INCREMENT:for (int i = 0; i < K_CONST; i++) {
    #pragma HLS pipeline
            vote_list[label_list[i]] += 1;
        }

        LabelType max_vote;
        max_vote = 0;

        VOTE:for (int i = 0; i < 10; i++) {
    #pragma HLS unroll
            if (vote_list[i] >= vote_list[max_vote]) {
                max_vote = i;
            }
        }

        result_stream.write(max_vote);
    }
}

void inference(WholeDigitType local_training_set[NUM_TRAINING / PAR_FACTOR],
               hls::stream<WholeDigitType> &test_data_stream_in,
               hls::stream<WholeDigitType> &test_data_stream_out,
               hls::stream<int> &knn_set_stream_in,
               hls::stream<int> &knn_set_stream_out,
               LabelType label_in) {

    // This array stores K minimum distances per training set
    int knn_set[NUM_LANE * K_CONST];
    int min_distance_list[K_CONST];
    int label_list[K_CONST];

    WholeDigitType test_inst = test_data_stream_in.read();

    for (int i = 0; i < NUM_LANE * K_CONST; i++) {
#pragma HLS unroll
        knn_set[i] = 256;
    }
    for (int i = 0; i < K_CONST; i++) {
        min_distance_list[i] = knn_set_stream_in.read();
    }
    for (int i = 0; i < K_CONST; i++) {
        label_list[i] = knn_set_stream_in.read();
    }

    TRAINING_LOOP:for (int i = 0; i < NUM_TRAINING / PAR_FACTOR / NUM_LANE; i++) {
#pragma HLS pipeline
        LANES:for (int j = 0; j < NUM_LANE; j++) {
#pragma HLS unroll
            WholeDigitType train_inst = local_training_set[j * NUM_TRAINING / PAR_FACTOR / NUM_LANE + i];
            update_knn(test_inst, train_inst, &knn_set[j * K_CONST]);
        }
    }

    knn_vote_small(knn_set, min_distance_list, label_list, label_in);

    for (int j = 0; j < K_CONST; j++) {
        knn_set_stream_out.write(min_distance_list[j]);
    }
    for (int j = 0; j < K_CONST; j++) {
        knn_set_stream_out.write(label_list[j]);
    }

    test_data_stream_out.write(test_inst);
}

// top-level hardware function
void digit_rec(WholeDigitType global_training_set[NUM_TRAINING],
               WholeDigitType global_test_set[NUM_TEST],
               LabelType global_results[NUM_TEST]) {

    const int unroll_factor = PAR_FACTOR * NUM_LANE;
    static WholeDigitType training_set[NUM_TRAINING];
#pragma HLS array_partition variable = training_set block factor = unroll_factor dim = 0

    static hls::stream<WholeDigitType> test_stream_set[PAR_FACTOR + 1];
    static hls::stream<int> knn_set_stream[PAR_FACTOR + 1];
    static hls::stream<LabelType> result_stream;

    LOAD_DATA:for (int i = 0; i < PAR_FACTOR; i++) {
#pragma HLS pipeline
        read_training_data_block(global_training_set, &training_set[i * NUM_TRAINING / PAR_FACTOR], i);
    }
    read_test_data(global_test_set, test_stream_set[0]);

    MAIN_LOOP:for (int t = 0; t < NUM_TEST; t++) {
        for (int i = 0; i < 2 * K_CONST; i++) {
            knn_set_stream[0].write(0);
        }

        for (int p = 0; p < PAR_FACTOR; p++) {
#pragma HLS pipeline
            inference(&training_set[p * NUM_TRAINING / PAR_FACTOR],
                      test_stream_set[p], test_stream_set[p + 1],
                      knn_set_stream[p], knn_set_stream[p + 1],
                      p / (PAR_FACTOR / 10));
        }

        test_stream_set[PAR_FACTOR].read();
    }

    knn_vote_final(knn_set_stream[PAR_FACTOR], result_stream);
    write_result(result_stream, global_results);
}
