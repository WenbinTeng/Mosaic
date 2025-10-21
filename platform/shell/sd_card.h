#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include "ff.h"
#include "xil_printf.h"
#include "xstatus.h"

int sd_init(FATFS *fatfs);
int sd_load_partial(const char *file_name, u32 byte_len, u64 des_addr);

#endif
