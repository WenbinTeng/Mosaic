#include "top_in.hpp"

namespace top_in_space {

void top_in(
    feature_t                   img[IMG_CH][IMG_H][IMG_W],
    weight_t                    fc1_weight[FC1_SIZE][CONV_SIZE],
    weight_t                    fc2_weight[FC2_SIZE][FC1_SIZE],
    weight_t                    fc3_weight[FC3_SIZE][FC2_SIZE],
    bias_t                      fc1_bias[FC1_SIZE],
    bias_t                      fc2_bias[FC2_SIZE],
    bias_t                      fc3_bias[FC3_SIZE],
    hls::stream<feature_t>&     img_stream,
    hls::stream<weight_t>&      fc1_weight_stream,
    hls::stream<bias_t>&        fc1_bias_stream,
    hls::stream<weight_t>&      fc2_weight_stream,
    hls::stream<bias_t>&        fc2_bias_stream,
    hls::stream<weight_t>&      fc3_weight_stream,
    hls::stream<bias_t>&        fc3_bias_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE m_axi port=img offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc1_weight offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc2_weight offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc3_weight offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc1_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc2_bias offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=fc3_bias offset=slave bundle=data
#pragma HLS INTERFACE axis port=img_stream
#pragma HLS INTERFACE axis port=fc1_weight_stream
#pragma HLS INTERFACE axis port=fc1_bias_stream
#pragma HLS INTERFACE axis port=fc2_weight_stream
#pragma HLS INTERFACE axis port=fc2_bias_stream
#pragma HLS INTERFACE axis port=fc3_weight_stream
#pragma HLS INTERFACE axis port=fc3_bias_stream

    for (int c = 0; c < IMG_CH; c++) {
        for (int i = 0; i < IMG_H; i++) {
            for (int j = 0; j < IMG_W; j++) {
                feature_t pixel = img[i][j];
                img_stream.write(pixel);
            }
        }
    }

    for (int i = 0; i < FC1_SIZE; i++) {
        for (int j = 0; j < CONV_SIZE; j++) {
            weight_t w = fc1_weight[i][j];
            fc1_weight_stream.write(w);
        }
    }

    for (int i = 0; i < FC1_SIZE; i++) {
        bias_t b = fc1_bias[i];
        fc1_bias_stream.write(b);
    }

    for (int i = 0; i < FC2_SIZE; i++) {
        for (int j = 0; j < FC1_SIZE; j++) {
            weight_t w = fc2_weight[i][j];
            fc2_weight_stream.write(w);
        }
    }

    for (int i = 0; i < FC2_SIZE; i++) {
        bias_t b = fc2_bias[i];
        fc2_bias_stream.write(b);
    }

    for (int i = 0; i < FC3_SIZE; i++) {
        for (int j = 0; j < FC2_SIZE; j++) {
            weight_t w = fc3_weight[i][j];
            fc3_weight_stream.write(w);
        }
    }

    for (int i = 0; i < FC3_SIZE; i++) {
        bias_t b = fc3_bias[i];
        fc3_bias_stream.write(b);
    }
}

}  // namespace top_in_space