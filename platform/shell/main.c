#include "configure.h"
#include "gpio.h"
#include "platform.h"
#include "sd_card.h"
#include "xhls_ip.h"

#include "xil_cache.h"
#include "xil_printf.h"
#include "xiltimer.h"
#include "xparameters.h"

#include "stdlib.h"

#define COUNTS_PER_SECOND XPAR_CPU_CORTEXA53_0_TIMESTAMP_CLK_FREQ
static FATFS fatfs;
XGpioPs_Config *GpioConfigPtr;
XGpioPs psGpioInstancePtr;

void XAccel_Set_Val(XHls_ip *InstancePtr, u64 Data, int offset) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, offset, (u32)Data);
}

int main() {

    init_platform();
    Xil_DCacheInvalidate();

    sd_init(&fatfs);
    gpio_init(GpioConfigPtr, &psGpioInstancePtr);

    slot_init(&psGpioInstancePtr, 78, 81);
    slot_init(&psGpioInstancePtr, 79, 82);
    slot_init(&psGpioInstancePtr, 80, 83);

    XFpga XFpgaInstance = {0U};
    XTime tstart, tend;
    s32 status = XFpga_Initialize(&XFpgaInstance);
    if (status != XST_SUCCESS) {
        xil_printf("  ERROR: Failed to init FPGA.\n\r");
        return -1;
    }

    float *input = (float *)memalign(8, 32 * 32 * sizeof(float));
    for (int i = 0; i < 32 * 32; i++) {
        input[i] = (float)rand() / RAND_MAX;
    }

    float *weight_0 = (float *)memalign(8, 6 * 5 * 5 * sizeof(float));
    for (int i = 0; i < 6 * 5 * 5; i++) {
        weight_0[i] = (float)rand() / RAND_MAX;
    }

    float *bias_0 = (float *)memalign(8, 6 * sizeof(float));
    for (int i = 0; i < 6; i++) {
        bias_0[i] = (float)rand() / RAND_MAX;
    }

    float *t0 = (float *)memalign(8, 6 * 14 * 14 * sizeof(float));
    for (int i = 0; i < 6 * 14 * 14; i++) {
        t0[i] = 0.0;
    }

    float ***weight_0 = (float ***)memalign(8, 6 * sizeof(float **));
    for (int i = 0; i < 6; i++) {
        weight_0[i] = (float **)memalign(8, 5 * sizeof(float *));
        for (int j = 0; j < 5; j++) {
            weight_0[i][j] = (float *)memalign(8, 5 * sizeof(float));
            for (int k = 0; k < 5; k++) {
                weight_0[i][j][k] = (float)rand();
            }
        }
    }

    float *bias_0 = (float *)memalign(8, 6 * sizeof(float));
    for (int i = 0; i < 6; i++) {
        bias_0[i] = (float)rand();
    }

    float ***t0 = (float ***)memalign(8, 6 * sizeof(float **));
    for (int i = 0; i < 6; i++) {
        t0[i] = (float **)memalign(8, 14 * sizeof(float *));
        for (int j = 0; j < 14; j++) {
            t0[i][j] = (float *)memalign(8, 14 * sizeof(float));
        }
    }

    float ****weight_1 = (float ****)memalign(8, 16 * sizeof(float ***));
    for (int i = 0; i < 16; i++) {
        weight_1[i] = (float ***)memalign(8, 6 * sizeof(float **));
        for (int j = 0; j < 6; j++) {
            weight_1[i][j] = (float **)memalign(8, 5 * sizeof(float *));
            for (int k = 0; k < 5; k++) {
                weight_1[i][j][k] = (float *)memalign(8, 5 * sizeof(float));
                for (int p = 0; p < 5; p++) {
                    weight_1[i][j][k][p] = (float)rand();
                }
            }
        }
    }

    float *bias_1 = (float *)memalign(8, 16 * sizeof(float));
    for (int i = 0; i < 16; i++) {
        bias_1[i] = (float)rand();
    }

    float ***t1 = (float ***)memalign(8, 16 * sizeof(float **));
    for (int i = 0; i < 16; i++) {
        t1[i] = (float **)memalign(8, 5 * sizeof(float *));
        for (int j = 0; j < 5; j++) {
            t1[i][j] = (float *)memalign(8, 5 * sizeof(float));
        }
    }

    float ****conv_weight_2 = (float ****)memalign(8, 120 * sizeof(float ***));
    for (int i = 0; i < 120; i++) {
        conv_weight_2[i] = (float ***)memalign(8, 16 * sizeof(float **));
        for (int j = 0; j < 16; j++) {
            conv_weight_2[i][j] = (float **)memalign(8, 5 * sizeof(float *));
            for (int k = 0; k < 5; k++) {
                conv_weight_2[i][j][k] = (float *)memalign(8, 5 * sizeof(float));
                for (int p = 0; p < 5; p++) {
                    conv_weight_2[i][j][k][p] = (float)rand();
                }
            }
        }
    }

    float *conv_bias_2 = (float *)memalign(8, 120 * sizeof(float));
    for (int i = 0; i < 120; i++) {
        conv_bias_2[i] = (float)rand();
    }

    float **fc_weight_2 = (float **)memalign(8, 10 * sizeof(float *));
    for (int i = 0; i < 10; i++) {
        fc_weight_2[i] = (float *)memalign(8, 120 * sizeof(float));
        for (int j = 0; j < 120; j++) {
            fc_weight_2[i][j] = (float)rand();
        }
    }

    float *fc_bias_2 = (float *)memalign(8, 10 * sizeof(float));
    for (int i = 0; i < 10; i++) {
        fc_bias_2[i] = (float)rand();
    }

    float *output = (float *)memalign(8, 10 * sizeof(float));
    for (int i = 0; i < 10; i++) {
        output[i] = 1.0;
    }

    XHls_ip slot_0_ip;
    XHls_ip_Config slot_0_ip_config;
    slot_0_ip_config.DeviceId = XPAR_SLOT_0_LAYER_CONV1_POOL2_0_DEVICE_ID;
    slot_0_ip_config.Axilites_BaseAddress = XPAR_SLOT_0_LAYER_CONV1_POOL2_0_S_AXI_CONTROL_BASEADDR;
    int ip_status = XHls_ip_CfgInitialize(&slot_0_ip, &slot_0_ip_config);
    if (ip_status != XST_SUCCESS) {
        xil_printf("  [ERROR]: Failed to init slot 0.\n\r");
        return 1
    };
    XAccel_Set_Val(&slot_0_ip, (u64)input, 0x18);
    XAccel_Set_Val(&slot_0_ip, (u64)weight_0, 0x20);
    XAccel_Set_Val(&slot_0_ip, (u64)bias_0, 0x28);
    XAccel_Set_Val(&slot_0_ip, (u64)t0, 0x30);

    XHls_ip slot_1_ip;
    XHls_ip_Config slot_1_ip_config;
    slot_1_ip_config.DeviceId = XPAR_SLOT_1_LAYER_CONV3_POOL4_0_DEVICE_ID;
    slot_1_ip_config.Axilites_BaseAddress = XPAR_SLOT_1_LAYER_CONV3_POOL4_0_S_AXI_CONTROL_BASEADDR;
    ip_status = XHls_ip_CfgInitialize(&slot_1_ip, &slot_1_ip_config);
    if (ip_status != XST_SUCCESS) {
        xil_printf("  [ERROR]: Failed to init slot 1.\n\r");
        return 1;
    }
    XAccel_Set_Val(&slot_1_ip, (u64)t0, 0x18);
    XAccel_Set_Val(&slot_1_ip, (u64)weight_1, 0x20);
    XAccel_Set_Val(&slot_1_ip, (u64)bias_1, 0x28);
    XAccel_Set_Val(&slot_1_ip, (u64)t1, 0x30);

    XHls_ip slot_2_ip;
    XHls_ip_Config slot_2_ip_config;
    slot_2_ip_config.DeviceId = XPAR_SLOT_2_LAYER_CONV5_FC6_0_DEVICE_ID;
    slot_2_ip_config.Axilites_BaseAddress = XPAR_SLOT_2_LAYER_CONV5_FC6_0_S_AXI_CONTROL_BASEADDR;
    ip_status = XHls_ip_CfgInitialize(&slot_2_ip, &slot_2_ip_config);
    if (ip_status != XST_SUCCESS) {
        xil_printf("  [ERROR]: Failed to init slot 2.\n\r");
        return 1;
    }
    XAccel_Set_Val(&slot_2_ip, (u64)t1, 0x18);
    XAccel_Set_Val(&slot_2_ip, (u64)conv_weight_2, 0x20);
    XAccel_Set_Val(&slot_2_ip, (u64)conv_bias_2, 0x28);
    XAccel_Set_Val(&slot_2_ip, (u64)fc_weight_2, 0x30);
    XAccel_Set_Val(&slot_2_ip, (u64)fc_bias_2, 0x38);
    XAccel_Set_Val(&slot_2_ip, (u64)output, 0x40);

    XHls_ip_Start(&slot_0_ip);
    while (!XHls_ip_IsDone(&slot_0_ip))
        ;
    XHls_ip_Start(&slot_1_ip);
    while (!XHls_ip_IsDone(&slot_1_ip))
        ;
    XHls_ip_Start(&slot_2_ip);
    while (!XHls_ip_IsDone(&slot_2_ip))
        ;

    Xil_DCacheInvalidate();

    xil_printf("  [INFO ] : End of inference.\n\r");
    for (int i = 0; i < 100; i++) {
        xil_printf("%d ", c[i]);
    }
    xil_printf("...\n\r");

    xil_printf("  INFO: Configuring slot 0 ...\n\r");
    XTime_GetTime(&tstart);
    reconfig_slot(XFpgaInstance, "0:/pr_0.bin", 1607504, psGpioInstancePtr, 78, 81);
    XTime_GetTime(&tend);
    xil_printf("  INFO: Configure time: %d us.\n\r", (tend - tstart) * 1000000 / COUNTS_PER_SECOND);

    xil_printf("  INFO: End of inference.\n\r");
    for (int i = 0; i < 10; i++) {
        xil_printf("%d ", (int)output[i]);
    }
    xil_printf("\n\r");

    cleanup_platform();
    return 0;
}
