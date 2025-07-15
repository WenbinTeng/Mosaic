#include "scale.hpp"

namespace scale_space {

void scale(
    hls::stream<data_t> &in_stream,
    hls::stream<data_t> &out_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
    
    data_t local_data[IMAGE_HEIGHT][IMAGE_WIDTH];

    static int factor = 0;
    const int height_list[12] = {200, 167, 139, 116, 96, 80, 67, 56, 47, 39, 32, 27};
    const int width_list[12] = {267, 222, 185, 154, 129, 107, 89, 74, 62, 52, 43, 36};
    int h1 = IMAGE_HEIGHT;
    int w1 = IMAGE_WIDTH;
    int h2 = height_list[factor];
    int w2 = width_list[factor];
    int x_ratio = (int)((w1 << 16) / w2) + 1;
    int y_ratio = (int)((h1 << 16) / h2) + 1;

    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
#pragma HLS PIPELINE
            data_t tmp_data = in_stream.read();
            local_data[i][j] = tmp_data;
        }
    }

    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
#pragma HLS PIPELINE
            if (j < w2 && i < h2) {
                int x = (i * y_ratio) >> 16;
                int y = (j * x_ratio) >> 16;
                out_stream.write(local_data[x][y]);
            }
        }
    }

    factor++;
}

}  // namespace scale_space