#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include <malloc.h>

#include "gpio.h"
#include "sd_card.h"
#include "types.h"

#include "xilfpga.h"

int slot_init(XGpioPs *psGpioInstancePtr, int decouple_id, int reset_id);
int reconfig_slot(XFpga XFpgaInstance, char *bin_name, u32 bin_size, XGpioPs psGpioInstancePtr,
                  int decouple_id, int reset_id);

#endif
