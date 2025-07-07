#include "zculling.hpp"

namespace zculling_space {

void zculling(
    hls::stream<fragment_t> &fragment_stream,
    hls::stream<pixel_t> &pixel_stream
) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=fragment_stream
#pragma HLS INTERFACE axis port=pixel_stream

    static bit8 z_buffer[MAX_X][MAX_Y];
#pragma HLS ARRAY_PARTITION variable=z_buffer type=complete dim=1

    static bit16 index = 0;
    if (index == 0) {
        for (bit16 j = 0; j < MAX_Y; j++) {
#pragma HLS PIPELINE
            for (bit16 i = 0; i < MAX_X; i++) {
                z_buffer[i][j] = 255;
            }
        }
    }

    fragment_t frag = fragment_stream.read();

    if (frag.z < z_buffer[frag.x][frag.y]) {
        pixel_t pixel;
        pixel.x = frag.x;
        pixel.y = frag.y;
        pixel.color = frag.color;
        z_buffer[frag.x][frag.y] = frag.z;
        pixel_stream.write(pixel);
    }

    index = (index + 1) % NUM_3D_TRI;
}

}  // namespace zculling_space