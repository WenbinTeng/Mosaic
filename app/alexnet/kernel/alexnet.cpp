#include "alexnet.hpp"

void layer_c1(volatile DTYPE input[3][227][227],
              volatile DTYPE weights[96][3][11][11],
              volatile DTYPE bias[96],
              volatile DTYPE output[96][55][55]) {
    DTYPE sum;

    DTYPE output_buff[55][55];
#pragma HLS ARRAY_PARTITION variable=output_buff type=cyclic factor=1 dim=2

    DTYPE input_buff[11];
#pragma HLS ARRAY_PARTITION variable=input_buff type=complete dim=1

    DTYPE partial_sum[11];
#pragma HLS ARRAY_PARTITION variable=partial_sum type=complete dim=1

    DTYPE weights_buff[3][11][11];
#pragma HLS ARRAY_PARTITION variable=weights_buff type=complete dim=3

    DTYPE bias_buff;

    output_loop:for (int co = 0; co < 96; co++) {
        for (int i = 0; i < 3; i++)
        for (int j = 0; j < 11; j++)
        for (int k = 0; k < 11; k++) {
#pragma HLS PIPELINE
            weights_buff[i][j][k] = weights[co][i][j][k];
        }
        bias_buff = bias[co];

        h_loop:for (int h = 0; h < 55; h++) {
            w_loop:for (int w = 0; w < 55; w++) {
                int ch = h * 4;
                int cw = w * 4;
                sum = 0.00f;

                input_loop:for (int ci = 0; ci < 3; ci++) {
                    for (int i = 0; i < 11; i++) {
                        for (int j = 0; j < 11; j++) {
#pragma HLS PIPELINE
                            input_buff[j] = input[ci][ch + i][cw + j];
                        }

                        partial_sum[0] = weights_buff[ci][i][0] * input_buff[0];
                        partial_sum[1] = weights_buff[ci][i][1] * input_buff[1];
                        partial_sum[2] = weights_buff[ci][i][2] * input_buff[2];
                        partial_sum[3] = weights_buff[ci][i][3] * input_buff[3];
                        partial_sum[4] = weights_buff[ci][i][4] * input_buff[4];
                        partial_sum[5] = weights_buff[ci][i][5] * input_buff[5];
                        partial_sum[6] = weights_buff[ci][i][6] * input_buff[6];
                        partial_sum[7] = weights_buff[ci][i][7] * input_buff[7];
                        partial_sum[8] = weights_buff[ci][i][8] * input_buff[8];
                        partial_sum[9] = weights_buff[ci][i][9] * input_buff[9];
                        partial_sum[10] = weights_buff[ci][i][10] * input_buff[10];

                        sum +=
                            partial_sum[0] + partial_sum[1] + partial_sum[2] +
                            partial_sum[3] + partial_sum[4] + partial_sum[5] +
                            partial_sum[6] + partial_sum[7] + partial_sum[8] +
                            partial_sum[9] + partial_sum[10];
                    }
                }
                sum += bias_buff;
                output_buff[h][w] = (sum > 0) ? sum : 0.0;
            }
        }

        for (int h = 0; h < 55; h++)
        for (int w = 0; w < 55; w++) {
#pragma HLS PIPELINE
            output[co][h][w] = output_buff[h][w];
        }
    }
}

void layer_s1(volatile DTYPE input[96][55][55],
              volatile DTYPE output[96][31][31]) {
    DTYPE data;
    bool cond;

    for (int c = 0; c < 96; c++)
    for (int h = 0; h < 27; h++)
    for (int w = 0; w < 27; w++) {
#pragma HLS PIPELINE
        DTYPE max_value = -1000000000000.0;
        for (int i = 0; i < 3; i++) {
#pragma HLS PIPELINE
            for (int j = 0; j < 3; j++) {
#pragma HLS PIPELINE
                data = input[c][(h * 2) + i][(w * 2) + j];
                cond = (max_value > data);
                max_value = (max_value > data) ? max_value : data;
            }
        }
        output[c][h + 2][w + 2] = (max_value > 0) ? max_value : 0.0;
    }
}

void layer_c2(volatile DTYPE input[96][31][31],
              volatile DTYPE weights[256][96][5][5],
              volatile DTYPE bias[256],
              volatile DTYPE output[256][27][27]) {
    DTYPE sum;

    DTYPE output_buff[27][27];
#pragma HLS ARRAY_PARTITION variable=output_buff type=cyclic factor=1 dim=2

    DTYPE input_buff[5];
#pragma HLS ARRAY_PARTITION variable=input_buff type=complete dim=1

    DTYPE partial_sum[5];
#pragma HLS ARRAY_PARTITION variable=partial_sum type=complete dim=1

    DTYPE weights_buff[96][5][5];
#pragma HLS ARRAY_PARTITION variable=weights_buff type=complete dim=3

    DTYPE bias_buff;

    output_loop:for (int co = 0; co < 256; co++) {
        for (int i = 0; i < 96; i++)
        for (int j = 0; j < 5; j++)
        for (int k = 0; k < 5; k++) {
#pragma HLS PIPELINE
            weights_buff[i][j][k] = weights[co][i][j][k];
        }
        bias_buff = bias[co];

        h_loop:for (int h = 0; h < 27; h++) {
            w_loop:for (int w = 0; w < 27; w++) {
                sum = 0.00f;

                input_loop:for (int ci = 0; ci < 96; ci++) {
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; j < 5; j++) {
#pragma HLS PIPELINE
                            input_buff[j] = input[ci][h + i][w + j];
                        }

                        partial_sum[0] = weights_buff[ci][i][0] * input_buff[0];
                        partial_sum[1] = weights_buff[ci][i][1] * input_buff[1];
                        partial_sum[2] = weights_buff[ci][i][2] * input_buff[2];
                        partial_sum[3] = weights_buff[ci][i][3] * input_buff[3];
                        partial_sum[4] = weights_buff[ci][i][4] * input_buff[4];

                        sum += partial_sum[0] + partial_sum[1] +
                               partial_sum[2] + partial_sum[3] + partial_sum[4];
                    }
                }
                sum += bias_buff;
                output_buff[h][w] = (sum > 0) ? sum : 0.0;
            }
        }

        for (int h = 0; h < 27; h++)
        for (int w = 0; w < 27; w++) {
#pragma HLS PIPELINE
            output[co][h][w] = output_buff[h][w];
        }
    }
}

void layer_s2(volatile DTYPE input[256][27][27],
              volatile DTYPE output[256][15][15]) {
    DTYPE data;
    bool cond;

    for (int c = 0; c < 256; c++)
    for (int h = 0; h < 13; h++)
    for (int w = 0; w < 13; w++) {
#pragma HLS PIPELINE
        DTYPE max_value = -1000000000000.0;
        for (int i = 0; i < 3; i++) {
#pragma HLS PIPELINE
            for (int j = 0; j < 3; j++) {
#pragma HLS PIPELINE
                data = input[c][(h * 2) + i][(w * 2) + j];
                cond = (max_value > data);
                max_value = (max_value > data) ? max_value : data;
            }
        }
        output[c][h][w] = (max_value > 0) ? max_value : 0.0;
    }
}

void layer_c3(volatile DTYPE input[256][15][15],
              volatile DTYPE weights[384][256][3][3],
              volatile DTYPE bias[384],
              volatile DTYPE output[384][15][15]) {
    DTYPE sum;

    DTYPE output_buff[15][15];
#pragma HLS ARRAY_PARTITION variable=output_buff type=cyclic factor=1 dim=2

    DTYPE input_buff[9];
#pragma HLS ARRAY_PARTITION variable=input_buff type=complete dim=1

    DTYPE partial_sum[9];
#pragma HLS ARRAY_PARTITION variable=partial_sum type=complete dim=1

    DTYPE weights_buff[256][9];
#pragma HLS ARRAY_PARTITION variable=weights_buff type=complete dim=2

    DTYPE bias_buff;

    output_loop:for (int co = 0; co < 384; co++) {
        for (int i = 0; i < 256; i++)
        for (int j = 0; j < 3; j++)
        for (int k = 0; k < 3; k++) {
#pragma HLS PIPELINE
            weights_buff[i][j * 3 + k] = weights[co][i][j][k];
        }
        bias_buff = bias[co];

        h_loop:for (int h = 0; h < 13; h++) {
            w_loop:for (int w = 0; w < 13; w++) {
                sum = 0.00f;

                input_loop:for (int ci = 0; ci < 256; ci++) {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            input_buff[i * 3 + j] = input[ci][h + i][w + j];
                        }
                    }

                    partial_sum[0] = weights_buff[ci][0] + input_buff[0];
                    partial_sum[1] = weights_buff[ci][1] + input_buff[1];
                    partial_sum[2] = weights_buff[ci][2] + input_buff[2];
                    partial_sum[3] = weights_buff[ci][3] + input_buff[3];
                    partial_sum[4] = weights_buff[ci][4] + input_buff[4];
                    partial_sum[5] = weights_buff[ci][5] + input_buff[5];
                    partial_sum[6] = weights_buff[ci][6] + input_buff[6];
                    partial_sum[7] = weights_buff[ci][7] + input_buff[7];
                    partial_sum[8] = weights_buff[ci][8] + input_buff[8];

                    sum += partial_sum[0] + partial_sum[1] + partial_sum[2] +
                           partial_sum[3] + partial_sum[4] + partial_sum[5] +
                           partial_sum[6] + partial_sum[7] + partial_sum[8];
                }
                sum += bias_buff;
                output_buff[h][w] = (sum > 0) ? sum : 0.0;
            }
        }

        for (int h = 0; h < 13; h++)
        for (int w = 0; w < 13; w++) {
#pragma HLS PIPELINE
            output[co][h + 1][w + 1] = output_buff[h][w];
        }
    }
}

void layer_c4(volatile DTYPE input[384][15][15],
              volatile DTYPE weights[384][384][3][3],
              volatile DTYPE bias[384],
              volatile DTYPE output[384][15][15]) {
    DTYPE sum;

    DTYPE output_buff[15][15];
#pragma HLS ARRAY_PARTITION variable=output_buff type=cyclic factor=1 dim=2

    DTYPE input_buff[9];
#pragma HLS ARRAY_PARTITION variable=input_buff type=complete dim=1

    DTYPE partial_sum[9];
#pragma HLS ARRAY_PARTITION variable=partial_sum type=complete dim=1

    DTYPE weights_buff[384][9];
#pragma HLS ARRAY_PARTITION variable=weights_buff type=complete dim=2

    DTYPE bias_buff;

    output_loop:for (int co = 0; co < 384; co++) {
        for (int i = 0; i < 384; i++)
        for (int j = 0; j < 3; j++)
        for (int k = 0; k < 3; k++) {
#pragma HLS PIPELINE
            weights_buff[i][j * 3 + k] = weights[co][i][j][k];
        }
        bias_buff = bias[co];

        h_loop:for (int h = 0; h < 13; h++) {
            w_loop:for (int w = 0; w < 13; w++) {
                sum = 0.00f;

                input_loop:for (int ci = 0; ci < 384; ci++) {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            input_buff[i * 3 + j] = input[ci][h + i][w + j];
                        }
                    }

                    partial_sum[0] = weights_buff[ci][0] + input_buff[0];
                    partial_sum[1] = weights_buff[ci][1] + input_buff[1];
                    partial_sum[2] = weights_buff[ci][2] + input_buff[2];
                    partial_sum[3] = weights_buff[ci][3] + input_buff[3];
                    partial_sum[4] = weights_buff[ci][4] + input_buff[4];
                    partial_sum[5] = weights_buff[ci][5] + input_buff[5];
                    partial_sum[6] = weights_buff[ci][6] + input_buff[6];
                    partial_sum[7] = weights_buff[ci][7] + input_buff[7];
                    partial_sum[8] = weights_buff[ci][8] + input_buff[8];

                    sum += partial_sum[0] + partial_sum[1] + partial_sum[2] +
                           partial_sum[3] + partial_sum[4] + partial_sum[5] +
                           partial_sum[6] + partial_sum[7] + partial_sum[8];
                }
                sum += bias_buff;
                output_buff[h][w] = (sum > 0) ? sum : 0.0;
            }
        }

        for (int h = 0; h < 13; h++)
        for (int w = 0; w < 13; w++) {
#pragma HLS PIPELINE
            output[co][h + 1][w + 1] = output_buff[h][w];
        }
    }
}

void layer_c5(volatile DTYPE input[384][15][15],
              volatile DTYPE weights[256][384][3][3],
              volatile DTYPE bias[256],
              volatile DTYPE output[256][13][13]) {
    DTYPE sum;

    DTYPE output_buff[15][15];
#pragma HLS ARRAY_PARTITION variable=output_buff type=cyclic factor=1 dim=2

    DTYPE input_buff[9];
#pragma HLS ARRAY_PARTITION variable=input_buff type=complete dim=1

    DTYPE partial_sum[9];
#pragma HLS ARRAY_PARTITION variable=partial_sum type=complete dim=1

    DTYPE weights_buff[384][9];
#pragma HLS ARRAY_PARTITION variable=weights_buff type=complete dim=2

    DTYPE bias_buff;

    output_loop:for (int co = 0; co < 256; co++) {
        for (int i = 0; i < 384; i++)
        for (int j = 0; j < 3; j++)
        for (int k = 0; k < 3; k++) {
#pragma HLS PIPELINE
            weights_buff[i][j * 3 + k] = weights[co][i][j][k];
        }
        bias_buff = bias[co];

        h_loop:for (int h = 0; h < 13; h++) {
            w_loop:for (int w = 0; w < 13; w++) {
                sum = 0.00f;

                input_loop:for (int ci = 0; ci < 384; ci++) {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            input_buff[i * 3 + j] = input[ci][h + i][w + j];
                        }
                    }

                    partial_sum[0] = weights_buff[ci][0] + input_buff[0];
                    partial_sum[1] = weights_buff[ci][1] + input_buff[1];
                    partial_sum[2] = weights_buff[ci][2] + input_buff[2];
                    partial_sum[3] = weights_buff[ci][3] + input_buff[3];
                    partial_sum[4] = weights_buff[ci][4] + input_buff[4];
                    partial_sum[5] = weights_buff[ci][5] + input_buff[5];
                    partial_sum[6] = weights_buff[ci][6] + input_buff[6];
                    partial_sum[7] = weights_buff[ci][7] + input_buff[7];
                    partial_sum[8] = weights_buff[ci][8] + input_buff[8];

                    sum += partial_sum[0] + partial_sum[1] + partial_sum[2] +
                           partial_sum[3] + partial_sum[4] + partial_sum[5] +
                           partial_sum[6] + partial_sum[7] + partial_sum[8];
                }
                sum += bias_buff;
                output_buff[h][w] = (sum > 0) ? sum : 0.0;
            }
        }

        for (int h = 0; h < 13; h++)
        for (int w = 0; w < 13; w++) {
#pragma HLS PIPELINE
            output[co][h][w] = output_buff[h][w];
        }
    }
}

void layer_s3(volatile DTYPE input[256][13][13],
              volatile DTYPE output[256][6][6]) {
    DTYPE data;
    bool cond;

    for (int c = 0; c < 256; c++)
    for (int h = 0; h < 6; h++)
    for (int w = 0; w < 6; w++) {
#pragma HLS PIPELINE
        DTYPE max_value = -1000000000000.0;
        for (int i = 0; i < 3; i++) {
#pragma HLS PIPELINE
            for (int j = 0; j < 3; j++) {
#pragma HLS PIPELINE
                data = input[c][(h * 2) + i][(w * 2) + j];
                cond = (max_value > data);
                max_value = (max_value > data) ? max_value : data;
            }
        }
        output[c][h][w] = (max_value > 0) ? max_value : 0.0;
    }
}

void layer_f1(volatile DTYPE input[256][6][6],
              volatile DTYPE weights[4096][256][6][6],
              volatile DTYPE bias[4096],
              volatile DTYPE output[4096]) {
    DTYPE sum;
    DTYPE sum0, sum1, sum2, sum3, sum4, sum5;

    DTYPE weights_buff[256][6][6];
#pragma HLS ARRAY_PARTITION variable=weights_buff type=complete dim=3

    DTYPE partial_sum[9216];
#pragma HLS ARRAY_PARTITION variable=partial_sum type=complete dim=1

    DTYPE bias_buff;

    output_loop:for (int co = 0; co < 4096; co++) {
        for (int i = 0; i < 256; i++)
        for (int j = 0; j < 6; j++)
        for (int k = 0; k < 6; k++) {
#pragma HLS PIPELINE
            weights_buff[i][j][k] = weights[co][i][j][k];
        }
        bias_buff = bias[co];

        input_loop:for (int ci = 0; ci < 256; ci++) {
            for (int i = 0; i < 6; i++) {
                for (int j = 0; j < 6; j++) {
#pragma HLS PIPELINE
                    partial_sum[(ci * 36) + (i * 6) + j] = weights_buff[ci][i][j] * input[ci][i][j];
                }
            }
        }

        sum0 = 0.00f;
        sum1 = 0.00f;
        sum2 = 0.00f;
        sum3 = 0.00f;
        sum4 = 0.00f;
        sum5 = 0.00f;
        for (int k = 0; k < 9216; k += 6) {
#pragma HLS PIPELINE
            sum0 += partial_sum[k + 0];
            sum1 += partial_sum[k + 1];
            sum2 += partial_sum[k + 2];
            sum3 += partial_sum[k + 3];
            sum4 += partial_sum[k + 4];
            sum5 += partial_sum[k + 5];
        }

        sum = sum0 + sum1 + sum2 + sum3 + sum4 + sum5;
        output[co] = (sum > 0) ? sum : 0.0;
    }
}

void layer_f2(volatile DTYPE input[4096],
              volatile DTYPE weights[4096][4096],
              volatile DTYPE bias[4096],
              volatile DTYPE output[4096]) {
    int n, c;
    DTYPE tmp;

    for (n = 0; n < 4096; n++) {
#pragma HLS PIPELINE
        tmp = 0.0f;
        for (c = 0; c < 4096; c++) {
#pragma HLS PIPELINE
            tmp += weights[n][c] * input[c];
        }
        output[n] = tmp + bias[n];
    }
}

void layer_f3(volatile DTYPE input[4096],
              volatile DTYPE weights[1000][4096],
              volatile DTYPE bias[1000],
              volatile DTYPE output[1000]) {
    int n, c;
    DTYPE tmp;

    for (n = 0; n < 1000; n++) {
#pragma HLS PIPELINE
        tmp = 0.0f;
        for (c = 0; c < 4096; c++) {
#pragma HLS PIPELINE
            tmp += weights[n][c] * input[c];
        }
        output[n] = tmp + bias[n];
    }
}
