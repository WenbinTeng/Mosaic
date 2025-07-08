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
#pragma HLS ARRAY_PARTITION variable=theta type=cyclic factor=PAR_FACTOR
#pragma HLS ARRAY_PARTITION variable=data type=cyclic factor=PAR_FACTOR
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
#pragma HLS ARRAY_PARTITION variable=grad type=cyclic factor=PAR_FACTOR
#pragma HLS ARRAY_PARTITION variable=data type=cyclic factor=PAR_FACTOR
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
#pragma HLS ARRAY_PARTITION variable=theta type=cyclic factor=PAR_FACTOR
#pragma HLS ARRAY_PARTITION variable=grad type=cyclic factor=PAR_FACTOR

    for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++) {
#pragma HLS PIPELINE
        for (int j = 0; j < PAR_FACTOR; j++)
            theta[i * PAR_FACTOR + j] += scale * grad[i * PAR_FACTOR + j];
    }
}

void sgd(
    vec_data_t data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
    vec_label_t label[NUM_TRAINING / L_VECTOR_SIZE],
    vec_feature_t theta[NUM_FEATURES / F_VECTOR_SIZE]
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=label offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=theta offset=slave bundle=data

    data_t data_buff[NUM_FEATURES];
    label_t label_buff[NUM_TRAINING];
    feature_t theta_local[NUM_FEATURES];
    feature_t grad_local[NUM_FEATURES];

    /*** 1. Read labels. ***/
    for (int i = 0; i < NUM_TRAINING / L_VECTOR_SIZE; i++) {
#pragma HLS PIPELINE
        vec_label_t tmp_label = label[i];
        for (int j = 0; j < L_VECTOR_SIZE; j++) {
            label_buff[i * L_VECTOR_SIZE + j] = tmp_label.range((j + 1) * LTYPE_WIDTH - 1, j * LTYPE_WIDTH);
        }
    }

    /*** 2. Main loop. ***/
    for (int i = 0; i < NUM_TRAINING; i++) {
#pragma HLS DATAFLOW
        label_t label = label_buff[i];
        for (int j = 0; j < NUM_FEATURES / D_VECTOR_SIZE; j++) {
#pragma HLS PIPELINE
            vec_data_t tmp_data = data[i * j];
            for (int k = 0; k < D_VECTOR_SIZE; k++) {
                data_buff[j * D_VECTOR_SIZE + k] = tmp_data.range((k + 1) * DTYPE_TWIDTH - 1, k * DTYPE_TWIDTH);
            }
        }
        feature_t dot = _dot_product(theta_local, data_buff);
        feature_t prob = _sigmoid(dot);
        _compute_gradient(grad_local, data_buff, prob - label);
        _update_parameter(theta_local, grad_local, -STEP_SIZE);
    }

    /*** 3. Write theta. ***/
    for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i++) {
#pragma HLS PIPELINE
        vec_feature_t tmp_theta = 0;
        for (int j = 0; j < F_VECTOR_SIZE; j++) {
            tmp_theta.range((j + 1) * FTYPE_TWIDTH - 1, j * FTYPE_TWIDTH) = theta_local[i * F_VECTOR_SIZE + j];
        }
        theta[i] = tmp_theta;
    }
}

}  // namespace sgd_space