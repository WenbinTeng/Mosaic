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
        VectorFeatureType tmp_data = data[i];
        for (int j = 0; j < D_VECTOR_SIZE; j++) {
            data_stream_stage_1[(i * D_VECTOR_SIZE + j) % PAR_FACTOR].write(tmp_data.range((j + 1) * DTYPE_TWIDTH - 1, j * DTYPE_TWIDTH));
            data_stream_stage_2[(i * D_VECTOR_SIZE + j) % PAR_FACTOR].write(tmp_data.range((j + 1) * DTYPE_TWIDTH - 1, j * DTYPE_TWIDTH));
        }
    }
}

void read_label_stream(VectorLabelType label[NUM_TRAINING / L_VECTOR_SIZE],
                       hls::stream<LabelType> &label_stream,
                       int index) {
#pragma HLS inline
    VectorLabelType tmp_label = label[index];
    for (int j = 0; j < L_VECTOR_SIZE; j++)
        label_stream.write(tmp_label.range((j + 1) * LTYPE_WIDTH - 1, j * LTYPE_WIDTH));
}

void init_theta(VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE],
                hls::stream<FeatureType> theta_stream_stage_1[PAR_FACTOR],
                hls::stream<FeatureType> theta_stream_stage_2[PAR_FACTOR]) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i++) {
#pragma HLS pipeline II = 1
        VectorFeatureType tmp_data = theta[i];
        for (int j = 0; j < F_VECTOR_SIZE; j++) {
            theta_stream_stage_1[(i * F_VECTOR_SIZE + j) % PAR_FACTOR].write(tmp_data.range((j + 1) * FTYPE_TWIDTH - 1, j * FTYPE_TWIDTH));
            theta_stream_stage_2[(i * F_VECTOR_SIZE + j) % PAR_FACTOR].write(tmp_data.range((j + 1) * FTYPE_TWIDTH - 1, j * FTYPE_TWIDTH));
        }
    }
}

void redirect_theta_stream(hls::stream<FeatureType> &result_stream,
                           hls::stream<FeatureType> &theta_stream_stage_1,
                           hls::stream<FeatureType> &theta_stream_stage_2) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS pipeline II = 1
        FeatureType tmp_theta = result_stream.read();
        theta_stream_stage_1.write(tmp_theta);
        theta_stream_stage_2.write(tmp_theta);
    }
}

void write_output(hls::stream<FeatureType> theta_stream[PAR_FACTOR],
                  VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE]) {
#pragma HLS inline
    for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i++) {
#pragma HLS pipeline II = 1
        VectorFeatureType tmp_theta = 0;
        for (int j = 0; j < F_VECTOR_SIZE; j++)
            tmp_theta.range((j + 1) * FTYPE_TWIDTH - 1, j * FTYPE_TWIDTH) = theta_stream[(i * F_VECTOR_SIZE + j) % PAR_FACTOR].read();
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

FeatureType compute_stage1(hls::stream<FeatureType> &theta_stream,
                           hls::stream<DataType> &data_stream) {
    FeatureType dot = 0;
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS pipeline II = 1
        FeatureType theta = theta_stream.read();
        DataType data = data_stream.read();
        dot += theta * data;
    }
    return dot;
}

void compute_stage2(hls::stream<FeatureType> &theta_stream,
                    hls::stream<DataType> &data_stream,
                    LabelType label,
                    hls::stream<FeatureType> &result_stream,
                    FeatureType prob) {
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS pipeline II = 1
        FeatureType theta = theta_stream.read();
        DataType data = data_stream.read();
        result_stream.write(theta + (-STEP_SIZE * (prob - label) * data));
    }
}

void spam_fil(VectorDataType data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
              VectorLabelType label[NUM_TRAINING / L_VECTOR_SIZE],
              VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE]) {
#pragma HLS dataflow

    static hls::stream<DataType> data_stream_stage_1[PAR_FACTOR];
    static hls::stream<DataType> data_stream_stage_2[PAR_FACTOR];
    static hls::stream<LabelType> label_stream;
    static hls::stream<FeatureType> theta_stream_stage_1[PAR_FACTOR];
    static hls::stream<FeatureType> theta_stream_stage_2[PAR_FACTOR];
    static hls::stream<FeatureType> result_stream[PAR_FACTOR];

    init_theta(theta, theta_stream_stage_1, theta_stream_stage_2);    
    TRAIN_LOOP:for (int training_id = 0; training_id < NUM_TRAINING; training_id++) {
#pragma HLS pipeline
        read_data_stream(&data[training_id * NUM_FEATURES / D_VECTOR_SIZE], data_stream_stage_1, data_stream_stage_2);
        if (training_id % L_VECTOR_SIZE == 0)
            read_label_stream(label, label_stream, training_id / L_VECTOR_SIZE);
        LabelType training_label = label_stream.read();
        FeatureType dot = 0;
        PAR_LOOP_1:for (int p = 0; p < PAR_FACTOR; p++) {
#pragma HLS unroll
            dot += compute_stage1(theta_stream_stage_1[p], data_stream_stage_1[p]);
        }
        FeatureType prob = Sigmoid(dot);
        PAR_LOOP_2:for (int p = 0; p < PAR_FACTOR; p++) {
#pragma HLS unroll
            compute_stage2(theta_stream_stage_2[p], data_stream_stage_2[p], training_label, result_stream[p], prob);
            redirect_theta_stream(result_stream[p], theta_stream_stage_1[p], theta_stream_stage_2[p]);
        }
    }
    write_output(theta_stream_stage_1, theta);
}
