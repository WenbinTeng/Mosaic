#include "sd_card.h"

int sd_init(FATFS *fatfs) {
    FRESULT rc;

    rc = f_mount(fatfs, "0:/", 1);
    if (rc) {
        xil_printf("  ERROR : f_mount returned %d\r\n", rc);
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int sd_load_partial(const char *file_name, u32 byte_len, u64 des_addr) {
    FIL fil;
    FRESULT rc;
    UINT br;

    rc = f_open(&fil, file_name, FA_READ);
    if (rc) {
        xil_printf("  ERROR : f_open returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_lseek(&fil, 0);
    if (rc) {
        xil_printf("  ERROR : f_lseek returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_read(&fil, (void *)des_addr, byte_len, &br);
    if (rc) {
        xil_printf("  ERROR : f_read returned %d\r\n", rc);
        return XST_FAILURE;
    }

    rc = f_close(&fil);
    if (rc) {
        xil_printf("  ERROR : f_close returned %d\r\n", rc);
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}
