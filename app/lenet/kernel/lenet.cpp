#include "lenet.h"

void layer_c1(volatile DTYPE input[32][32],
              volatile DTYPE weights[6][5][5],
              volatile DTYPE bias[6],
              volatile DTYPE output[6][28][28]) {
    int co, h, w, i, j;
    DTYPE tmp, sum;

    for (co = 0; co < 6; co++)
    for (h = 0; h < 28; h++)
    for (w = 0; w < 28; w++) {
        DTYPE sum = 0;
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
#pragma HLS PIPELINE
                sum += weights[co][i][j] * input[i+h][j+w];
            }
        }
        tmp = sum + bias[co];
        output[co][h][w] = (tmp > 0) ? tmp : 0.0;
    }
}

void layer_s2(volatile DTYPE input[6][28][28],
              volatile DTYPE output[6][14][14]) {
    int c, h, w, i, j;
    DTYPE data;
    bool cond;

    for (c = 0; c < 6; c++)
    for (h = 0; h < 14; h++)
    for (w = 0; w < 14; w++) {
        DTYPE max_value = -10000000.0;
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 2; j++) {
#pragma HLS PIPELINE
                data = input[c][(h*2)+i][(w*2)+j];
                cond = (max_value > input[c][(h*2)+i][(w*2)+j]);
                max_value = cond ? max_value : data;
            }
        }
        output[c][h][w] = (max_value > 0) ? max_value : 0.0;
    }
}

void layer_c3(volatile DTYPE input[6][14][14],
              volatile DTYPE weights[16][6][5][5],
              volatile DTYPE bias[16],
              volatile DTYPE output[16][10][10]) {

    int co, h, w, i, j, k, m, n, ci;
	DTYPE tmp;
	DTYPE sum0, sum1, sum2, sum3, sum4;
	DTYPE conv3_partial_sum[150];
#pragma HLS ARRAY_PARTITION variable=conv3_partial_sum cyclic factor=5 dim=1
	DTYPE weights_buff[6][5][5];
	DTYPE bias_buff;

	l0:for(co = 0; co < 16; co++) {
        for (ci = 0; ci < 6; ci++)
        for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
#pragma HLS PIPELINE
			weights_buff[ci][i][j] = weights[co][ci][i][j];
		bias_buff = bias[co];

		l1:for(h = 0; h < 10; h++) {
			l2:for(w = 0; w < 10; w++) {
				l3:for(ci = 0; ci < 6; ci++)
				l4:for(i = 0; i < 5; i++)
				l5:for(j = 0; j < 5; j++) {
#pragma HLS PIPELINE
					conv3_partial_sum[(ci*25)+(i*5)+j] = weights_buff[ci][i][j] * input[ci][i+h][j+w];
				}

				sum0 = 0.00f;
				sum1 = 0.00f;
				sum2 = 0.00f;
				sum3 = 0.00f;
				sum4 = 0.00f;
                l6:for (k = 0; k < 150; k += 5) {
#pragma HLS PIPELINE
					sum0 += conv3_partial_sum[k+0];
					sum1 += conv3_partial_sum[k+1];
					sum2 += conv3_partial_sum[k+2];
					sum3 += conv3_partial_sum[k+3];
					sum4 += conv3_partial_sum[k+4];
                }

                tmp = sum0 + sum1 + sum2 + sum3 + sum4 + bias_buff;
                output[co][h][w] = (tmp > 0) ? tmp : 0.0;
			}
		}
	}
}

void layer_s4(volatile DTYPE input[16][10][10],
              volatile DTYPE output[16][5][5]) {
    DTYPE data;
	bool cond;
	int c, h, w, i, j;

    for(c = 0; c < 16; c++)
	for(h = 0; h < 5; h++)
	for(w = 0; w < 5; w++) {
#pragma HLS PIPELINE
        DTYPE max_value = -1000000000000.0;
        for(i = 0; i < 2; i++) {
#pragma HLS PIPELINE
			for(j = 0; j < 2; j++) {
#pragma HLS PIPELINE
				data = input[c][(h*2)+i][(w*2)+j];
				cond = max_value > input[c][(h*2)+i][(w*2)+j];
				max_value = (max_value > data) ? max_value : data;
			}
		}
		output[c][h][w] = (max_value > 0) ? max_value : 0.0;
	}
}

void layer_c5(volatile DTYPE input[16][5][5],
              volatile DTYPE weights[120][16][5][5],
              volatile DTYPE bias[120],
              volatile DTYPE output[120]) {
    int co, i, j, k, m, n, ci;
	DTYPE tmp;
	DTYPE sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, sum9;
	DTYPE conv5_partial_sum[400];
#pragma HLS ARRAY_PARTITION variable=conv5_partial_sum cyclic factor=10 dim=1
	DTYPE weights_buff[16][5][5];
	DTYPE bias_buff;

    l0:for(co = 0; co < 120; co++) {
        for (ci = 0; ci < 6; ci++)
        for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
#pragma HLS PIPELINE
			weights_buff[ci][i][j] = weights[co][ci][i][j];
		bias_buff = bias[0];

        l1:for (ci = 0; ci < 16; ci++) {
			l2:for(i = 0; i < 5; i++) {
				l3:for(j = 0; j < 5; j++) {
#pragma HLS PIPELINE
					conv5_partial_sum[(ci*25)+(i*5)+j] = weights_buff[ci][i][j] * input[ci][i][j];
				}
			}
    	}

        sum0 = 0.00f;
		sum1 = 0.00f;
		sum2 = 0.00f;
		sum3 = 0.00f;
		sum4 = 0.00f;
		sum5 = 0.00f;
		sum6 = 0.00f;
		sum7 = 0.00f;
		sum8 = 0.00f;
		sum9 = 0.00f;
		l4:for(k = 0; k < 400; k += 10) {
#pragma HLS PIPELINE
			sum0 += conv5_partial_sum[k+0];
			sum1 += conv5_partial_sum[k+1];
			sum2 += conv5_partial_sum[k+2];
			sum3 += conv5_partial_sum[k+3];
			sum4 += conv5_partial_sum[k+4];
			sum5 += conv5_partial_sum[k+5];
			sum6 += conv5_partial_sum[k+6];
			sum7 += conv5_partial_sum[k+7];
			sum8 += conv5_partial_sum[k+8];
			sum9 += conv5_partial_sum[k+9];
		}

		tmp = sum0 + sum1 + sum2 + sum3 + sum4 +
              sum5 + sum6 + sum7 + sum8 + sum9 +
              bias_buff;

		output[co] = (tmp > 0) ? tmp : 0.0;
    }
}

void layer_f6(volatile DTYPE input[120],
              volatile DTYPE weights[84][120],
              volatile DTYPE bias[84],
              volatile DTYPE output[84]) {
    int n, c;
	DTYPE tmp;

    for(n = 0; n < 84; n++) {
#pragma HLS PIPELINE
    	tmp = 0.0f;
        for(c = 0; c < 120; c++) {
#pragma HLS PIPELINE
        	tmp += weights[n][c] * input[c];
        }
        output[n] = tmp + bias[n];
    }
}

void layer_output(volatile DTYPE input[84],
                  volatile DTYPE weights[10][84],
                  volatile DTYPE bias[10],
                  volatile DTYPE output[10]) {
    int n, c;
	DTYPE tmp;

    for(n = 0; n < 10; n++) {
#pragma HLS PIPELINE
    	tmp = 0.0f;
        for(c = 0; c < 84; c++) {
#pragma HLS PIPELINE
        	tmp += weights[n][c] * input[c];
        }
        output[n] = tmp + bias[n];
    }
}
