// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2023.2 (64-bit)
// Tool Version Limit: 2023.10
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
//
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of ap_return
//        bit 31~0 - ap_return[31:0] (Read)
// 0x18 : Data signal of input_r
//        bit 31~0 - input_r[31:0] (Read/Write)
// 0x1c : Data signal of input_r
//        bit 31~0 - input_r[63:32] (Read/Write)
// 0x20 : reserved
// 0x24 : Data signal of weights
//        bit 31~0 - weights[31:0] (Read/Write)
// 0x28 : Data signal of weights
//        bit 31~0 - weights[63:32] (Read/Write)
// 0x2c : reserved
// 0x30 : Data signal of bias
//        bit 31~0 - bias[31:0] (Read/Write)
// 0x34 : Data signal of bias
//        bit 31~0 - bias[63:32] (Read/Write)
// 0x38 : reserved
// 0x3c : Data signal of output_r
//        bit 31~0 - output_r[31:0] (Read/Write)
// 0x40 : Data signal of output_r
//        bit 31~0 - output_r[63:32] (Read/Write)
// 0x44 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XHLS_IP_CONTROL_ADDR_AP_CTRL 0x00
#define XHLS_IP_CONTROL_ADDR_GIE 0x04
#define XHLS_IP_CONTROL_ADDR_IER 0x08
#define XHLS_IP_CONTROL_ADDR_ISR 0x0c
#define XHLS_IP_CONTROL_ADDR_AP_RETURN 0x10
#define XHLS_IP_CONTROL_BITS_AP_RETURN 32
#define XHLS_IP_CONTROL_ADDR_INPUT_R_DATA 0x18
#define XHLS_IP_CONTROL_BITS_INPUT_R_DATA 64
#define XHLS_IP_CONTROL_ADDR_WEIGHTS_DATA 0x24
#define XHLS_IP_CONTROL_BITS_WEIGHTS_DATA 64
#define XHLS_IP_CONTROL_ADDR_BIAS_DATA 0x30
#define XHLS_IP_CONTROL_BITS_BIAS_DATA 64
#define XHLS_IP_CONTROL_ADDR_OUTPUT_R_DATA 0x3c
#define XHLS_IP_CONTROL_BITS_OUTPUT_R_DATA 64
