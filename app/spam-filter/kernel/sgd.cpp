/*===============================================================*/
/*                                                               */
/*                          sgd.cpp                              */
/*                                                               */
/*             Hardware function for spam filtering.             */
/*                                                               */
/*===============================================================*/

#include "sgd.hpp"

void read_data_stream(VectorDataType data[NUM_FEATURES / D_VECTOR_SIZE],
                      hls::stream<DataType> data_stream_stage_1[PAR_FACTOR],
                      hls::stream<DataType> data_stream_stage_2[PAR_FACTOR]) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES / D_VECTOR_SIZE; i++) {
#pragma HLS pipeline II = 1
        VectorDataType tmp_data = data[i];
        for (int j = 0; j < D_VECTOR_SIZE; j++) {
            data_stream_stage_1[(i * D_VECTOR_SIZE + j) % PAR_FACTOR].write(tmp_data.range((j + 1) * DTYPE_TWIDTH - 1, j * DTYPE_TWIDTH));
            data_stream_stage_2[(i * D_VECTOR_SIZE + j) % PAR_FACTOR].write(tmp_data.range((j + 1) * DTYPE_TWIDTH - 1, j * DTYPE_TWIDTH));
        }
    }
}

void read_label_stream(VectorLabelType label[NUM_TRAINING / L_VECTOR_SIZE],
                       hls::stream<LabelType> label_stream[PAR_FACTOR]) {
#pragma HLS inline
    for (int i = 0; i < NUM_TRAINING / L_VECTOR_SIZE; i++) {
        VectorLabelType tmp_label = label[i];
#pragma HLS pipeline II = 1
        for (int j = 0; j < L_VECTOR_SIZE; j++) {
            for (int p = 0; p < PAR_FACTOR; p++) {
#pragma HLS unroll
                label_stream[p].write(tmp_label.range((j + 1) * LTYPE_WIDTH - 1, j * LTYPE_WIDTH));
            }
        }
    }
}

void load_theta(FeatureType theta_local[NUM_FEATURES],
                hls::stream<FeatureType> theta_stream_stage_1[PAR_FACTOR],
                hls::stream<FeatureType> theta_stream_stage_2[PAR_FACTOR]) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES; i++) {
#pragma HLS pipeline II = 1
        FeatureType tmp_theta = theta_local[i];
        theta_stream_stage_1[i % PAR_FACTOR].write(tmp_theta);
        theta_stream_stage_2[i % PAR_FACTOR].write(tmp_theta);
    }
}

void update_theta(hls::stream<FeatureType> result_stream[PAR_FACTOR],
                  FeatureType theta_local[NUM_FEATURES]) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES; i++) {
#pragma HLS pipeline II = 1
        FeatureType tmp_theta = result_stream[i % PAR_FACTOR].read();
        theta_local[i] = tmp_theta;
    }
}

void write_output(FeatureType theta_local[NUM_FEATURES],
                  VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE]) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i++) {
#pragma HLS pipeline II = 1
        VectorFeatureType tmp_theta = 0;
        for (int j = 0; j < F_VECTOR_SIZE; j++) {
            tmp_theta.range((j+1)*FTYPE_TWIDTH-1, j*FTYPE_TWIDTH) = theta_local[i * F_VECTOR_SIZE + j];
        }
        theta[i] = tmp_theta;
    }
}

// values of sigmoid function stored in a look-up table
FeatureType use_LUT(FeatureType in) {
#pragma HLS inline
    IdxFixed index;
    if (in < 0) {
        in = -in;
        index = (IdxFixed)LUT_SIZE - (IdxFixed)(((TmpFixed)in) << (LUTIN_TWIDTH - LUTIN_IWIDTH));
    } else
        index = ((TmpFixed)in) << (LUTIN_TWIDTH - LUTIN_IWIDTH);
    return lut[index];
}

// Function to compute the probability. The Sigmoid function is implemented
// using BRAMS to store the value of Sigmoid function for values relevant to
// this application
FeatureType Sigmoid(FeatureType exponent) {
#pragma HLS inline
    if (exponent > 4)
        return 1.0;
    else if (exponent < -4)
        return 0.0;
    else {
        LutInFixed inLut = (LutInFixed)exponent;
        return use_LUT(inLut);
    }
}

void compute_stage_1(hls::stream<FeatureType> &theta_stream,
                     hls::stream<DataType> &data_stream,
                     hls::stream<FeatureType> &dot_stream) {
    FeatureType dot = 0;
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS pipeline II = 1
        FeatureType theta = theta_stream.read();
        DataType data = data_stream.read();
        dot += theta * data;
    }
    dot_stream.write(dot);
}

void compute_stage_2(hls::stream<FeatureType> dot_stream[PAR_FACTOR],
                     hls::stream<FeatureType> prob_stream[PAR_FACTOR]) {
    FeatureType dot = 0;
    for (int i = 0; i < PAR_FACTOR; i++) {
#pragma HLS pipeline
        dot += dot_stream[i].read();
    }
    FeatureType prob = Sigmoid(dot);
    for (int i = 0; i < PAR_FACTOR; i++) {
#pragma HLS pipeline
        prob_stream[i].write(prob);
    }
}

void compute_stage_3(hls::stream<FeatureType> &theta_stream,
                     hls::stream<DataType> &data_stream,
                     hls::stream<LabelType> &label_steam,
                     hls::stream<FeatureType> &prob_stream,
                     hls::stream<FeatureType> &result_stream) {
    FeatureType label = label_steam.read();
    FeatureType prob = prob_stream.read();
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS pipeline
        FeatureType theta = theta_stream.read();
        DataType data = data_stream.read();
        FeatureType result = theta + (-STEP_SIZE * (prob - label) * data);
        result_stream.write(result);
    }
}

void spam_fil(VectorDataType data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
              VectorLabelType label[NUM_TRAINING / L_VECTOR_SIZE],
              VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE]) {
    static FeatureType theta_local[NUM_FEATURES];
    static hls::stream<LabelType> label_stream[PAR_FACTOR];
    static hls::stream<DataType> data_stream_stage_1[PAR_FACTOR];
    static hls::stream<DataType> data_stream_stage_2[PAR_FACTOR];
    static hls::stream<FeatureType> theta_stream_stage_1[PAR_FACTOR];
    static hls::stream<FeatureType> theta_stream_stage_2[PAR_FACTOR];
    static hls::stream<FeatureType> dot_stream[PAR_FACTOR];
    static hls::stream<FeatureType> prob_stream[PAR_FACTOR];
    static hls::stream<FeatureType> result_stream[PAR_FACTOR];

    read_label_stream(label, label_stream);
    TRAIN_LOOP:for (int training_id = 0; training_id < NUM_TRAINING; training_id++) {
        read_data_stream(&data[training_id * NUM_FEATURES / D_VECTOR_SIZE], data_stream_stage_1, data_stream_stage_2);
        load_theta(theta_local, theta_stream_stage_1, theta_stream_stage_2);
        PAR_LOOP_1:for (int p = 0; p < PAR_FACTOR; p++) {
#pragma HLS unroll
            compute_stage_1(theta_stream_stage_1[p], data_stream_stage_1[p], dot_stream[p]);
        }
        compute_stage_2(dot_stream, prob_stream);
        PAR_LOOP_2:for (int p = 0; p < PAR_FACTOR; p++) {
#pragma HLS unroll
            compute_stage_3(theta_stream_stage_2[p], data_stream_stage_2[p], label_stream[p], prob_stream[p], result_stream[p]);
        }
        update_theta(result_stream, theta_local);
    }
    write_output(theta_local, theta);
}
