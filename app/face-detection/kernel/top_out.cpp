#include "top_out.hpp"

namespace top_out_space {

void top_out(
    hls::stream<result_t> &result_stream,
    int result_x[RESULT_SIZE],
    int result_y[RESULT_SIZE],
    int result_w[RESULT_SIZE],
    int result_h[RESULT_SIZE],
    int *result_size
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=result_stream
#pragma HLS INTERFACE m_axi depth=IMAGE_WIDTH port=data offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_x offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_y offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_w offset=slave bundle=data
#pragma HLS INTERFACE m_axi depth=RESULT_SIZE port=result_h offset=slave bundle=data
#pragma HLS INTERFACE m_axi port=result_size offset=slave bundle=data

    static int result_cnt = 0;
    for (int i = 0; i < RESULT_SIZE; i++) {
#pragma HLS PIPELINE
        result_t res = result_stream.read();
        rect_t r = res.r;
        if (res.result > 0) {
            result_x[result_cnt] = r.x;
            result_y[result_cnt] = r.y;
            result_w[result_cnt] = r.width;
            result_h[result_cnt] = r.height;
            result_cnt++;
        }
    }
    for (int i = result_cnt; i < RESULT_SIZE; i++) {
        result_x[i] = 0;
        result_y[i] = 0;
        result_w[i] = 0;
        result_h[i] = 0;
    }
    *result_size = result_cnt;
}

}  // namespace top_out_space