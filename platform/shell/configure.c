#include "configure.h"

int slot_init(XGpioPs *psGpioInstancePtr, int decouple_id, int reset_id) {
    // reset slot
    XGpioPs_SetDirectionPin(psGpioInstancePtr, reset_id, 0x1);
    XGpioPs_SetOutputEnablePin(psGpioInstancePtr, reset_id, 1);
    XGpioPs_WritePin(psGpioInstancePtr, reset_id, 1);
    // decouple slot
    XGpioPs_SetDirectionPin(psGpioInstancePtr, decouple_id, 0x1);
    XGpioPs_SetOutputEnablePin(psGpioInstancePtr, decouple_id, 1);
    XGpioPs_WritePin(psGpioInstancePtr, decouple_id, 0);
    return 0;
}

int reconfig_slot(XFpga XFpgaInstance, char *bin_name, u32 bin_size, XGpioPs psGpioInstancePtr,
                  int decouple_id, int reset_id) {
    char *p = (char *)memalign(8, PARTIAL_BITFILE_ALLOCATED_SPACE * sizeof(char));

    int sd_status = sd_load_partial(bin_name, bin_size, (u64)p);
    if (sd_status != XST_SUCCESS) {
        xil_printf("  [ERROR]: Failed to load bitstream from SD card.\n\r");
        free(p);
        return 1;
    } else {
        xil_printf("  [INFO ]: Loaded bitstream from SD card.\n\r");
    }

    XGpioPs_WritePin(&psGpioInstancePtr, decouple_id, 1);
    u32 fpga_status =
        XFpga_BitStream_Load(&XFpgaInstance, (UINTPTR)p, (UINTPTR)NULL, bin_size, XFPGA_PARTIAL_EN);
    if (fpga_status != XFPGA_SUCCESS) {
        xil_printf("  [ERROR]: Failed to configure slot %d.\n\r", decouple_id);
        free(p);
        return 1;
    } else {
        xil_printf("  [INFO ]: Configured slot %d.\n\r", decouple_id);
    }
    XGpioPs_WritePin(&psGpioInstancePtr, decouple_id, 0);
    XGpioPs_WritePin(&psGpioInstancePtr, reset_id, 0);
    usleep(100);
    XGpioPs_WritePin(&psGpioInstancePtr, reset_id, 1);

    free(p);
    return 0;
}