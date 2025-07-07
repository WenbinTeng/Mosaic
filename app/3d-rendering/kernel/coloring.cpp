#include "coloring.hpp"

namespace coloring_space {

void coloring(
    hls::stream<pixel_t> &pixel_stream,
    hls::stream<bit8> &frame_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=pixel_stream
#pragma HLS INTERFACE axis port=frame_stream

    static bit8 frame_buffer[MAX_X][MAX_Y];
#pragma HLS ARRAY_PARTITION variable=frame_buffer type=complete dim=1

    static bit16 index = 0;
    if (index == 0) {
        for (bit16 j = 0; j < MAX_Y; j++) {
#pragma HLS PIPELINE
            for (bit16 i = 0; i < MAX_X; i++) {
                frame_buffer[i][j] = 0;
            }
        }
    } else if (index == NUM_3D_TRI - 1) {
        for (bit16 j = 0; j < MAX_Y; j++) {
            for (bit16 i = 0; i < MAX_X; i++) {
#pragma HLS PIPELINE
                bit8 frame = frame_buffer[i][j];
                frame_stream.write(frame);
            }
        }
    }

    pixel_t pixel = pixel_stream.read();
    frame_buffer[pixel.x][pixel.y] = pixel.color;

    index = (index + 1) % NUM_3D_TRI;
}

}  // namespace coloring_space