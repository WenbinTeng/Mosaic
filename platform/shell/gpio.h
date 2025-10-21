#ifndef _GPIO_H_
#define _GPIO_H_

#include "xgpiops.h"
#include "xil_printf.h"

int gpio_init(XGpioPs_Config *GpioConfigPtr, XGpioPs *psGpioInstancePtr);

#endif
