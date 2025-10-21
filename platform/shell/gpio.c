#include "gpio.h"

int gpio_init(XGpioPs_Config *GpioConfigPtr, XGpioPs *psGpioInstancePtr) {
    GpioConfigPtr = XGpioPs_LookupConfig(XPAR_PSU_APM_0_DEVICE_ID);

    if (GpioConfigPtr == NULL) {
        xil_printf("  ERROR: Cannot acquire GPIO configure pointer. \n\r");
        return XST_FAILURE;
    }

    s32 status = XGpioPs_CfgInitialize(psGpioInstancePtr, GpioConfigPtr, GpioConfigPtr->BaseAddr);

    if (status != XST_SUCCESS) {
        xil_printf("  ERROR: Failed to init GPIO. \n\r");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}
