#include "sgd.hpp"

namespace sgd_space {

// values of sigmoid function stored in a look-up table
feature_t _use_LUT(feature_t in) {
#pragma HLS INLINE
    idx_fixed_t index;
    if (in < 0) {
        in = -in;
        index =
            (idx_fixed_t)LUT_SIZE -
            (idx_fixed_t)(((tmp_fixed_t)in) << (LUTIN_TWIDTH - LUTIN_IWIDTH));
    } else
        index = ((tmp_fixed_t)in) << (LUTIN_TWIDTH - LUTIN_IWIDTH);
    return lut[index];
}

// Function to compute the dot product of data (feature) vector and parameter vector
feature_t _dot_product(
    feature_t theta[NUM_FEATURES],
    data_t    data[NUM_FEATURES]
) {
#pragma HLS INLINE
    feature_t dot = 0;
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS PIPELINE
        for (int j = 0; j < PAR_FACTOR; j++) {
            feature_t term = theta[i*PAR_FACTOR+j] * data[i*PAR_FACTOR+j];
            dot += term;
        }
    }
    return dot;
}

// Function to compute the probability. The Sigmoid function is implemented using BRAMS 
// to store the value of Sigmoid function for values relevant to this application
feature_t _sigmoid(feature_t exponent) {
#pragma HLS INLINE
    if (exponent > 4)
        return 1.0;
    else if (exponent < -4)
        return 0.0;
    else {
        lutin_fixed_t inLut = (lutin_fixed_t)exponent;
        return _use_LUT(inLut);
    }
}

// Compute the gradient of the cost function
void _compute_gradient(
    feature_t grad[NUM_FEATURES],
    data_t data[NUM_FEATURES],
    feature_t scale
) {
#pragma HLS INLINE
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS PIPELINE
        for (int j = 0; j < PAR_FACTOR; j++)
            grad[i * PAR_FACTOR + j] = (scale * data[i * PAR_FACTOR + j]);
    }
}

// Update the parameter vector
void _update_parameter(
    feature_t theta[NUM_FEATURES],
    feature_t grad[NUM_FEATURES],
    feature_t scale)
{
#pragma HLS INLINE
    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS PIPELINE
        for (int j = 0; j < PAR_FACTOR; j++)
            theta[i * PAR_FACTOR + j] += scale * grad[i * PAR_FACTOR + j];
    }
}

void sgd(
    hls::stream<data_t> &data_stream,
    hls::stream<label_t> &label_stream,
    hls::stream<feature_t> &theta_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=data_stream
#pragma HLS INTERFACE axis port=label_stream
#pragma HLS INTERFACE axis port=theta_stream

    /*** Local data buffer ***/
    data_t data_local[NUM_FEATURES];
#pragma HLS ARRAY_PARTITION variable=data_local type=cyclic factor=PAR_FACTOR

    /*** Local label buffer ***/
    label_t label_local;

    /*** Local theta buffer ***/
    feature_t theta_local[NUM_FEATURES];
#pragma HLS ARRAY_PARTITION variable=theta_local type=cyclic factor=PAR_FACTOR

    /*** Local gradient buffer ***/
    feature_t grad_local[NUM_FEATURES];
#pragma HLS ARRAY_PARTITION variable=grad_local type=cyclic factor=PAR_FACTOR

    for (int i = 0; i < NUM_TRAINING; i++) {
#pragma HLS DATAFLOW
        for (int j = 0; j < NUM_FEATURES; j++) {
#pragma HLS PIPELINE
            data_local[j] = data_stream.read();
        }
        label_local = label_stream.read();
        feature_t dot = _dot_product(theta_local, data_local);
        feature_t prob = _sigmoid(dot);
        _compute_gradient(grad_local, data_local, prob - label);
        _update_parameter(theta_local, grad_local, -STEP_SIZE);
    }

    for (int i = 0; i < NUM_FEATURES; i++) {
#pragma HLS PIPELINE
        theta_stream.write(theta[i]);
    }
}

}  // namespace sgd_space