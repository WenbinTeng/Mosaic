/*===============================================================*/
/*                                                               */
/*                       face_detect.h                           */
/*                                                               */
/*     Hardware function for the Face Detection application.     */
/*                                                               */
/*===============================================================*/

#ifndef __FACE_DETECT_H__
#define __FACE_DETECT_H__

#include "hls_stream.h"

#include "typedef.h"

void face_detect(unsigned char Data[IMAGE_WIDTH],
                 int result_x[RESULT_SIZE],
                 int result_y[RESULT_SIZE],
                 int result_w[RESULT_SIZE],
                 int result_h[RESULT_SIZE],
                 int *result_size);

#endif