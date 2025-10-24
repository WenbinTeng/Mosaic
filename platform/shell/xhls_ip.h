// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2023.2 (64-bit)
// Tool Version Limit: 2023.10
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
//
// ==============================================================
#ifndef XHLS_IP_H
#define XHLS_IP_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_assert.h"
#include "xil_io.h"
#include "xil_types.h"
#include "xstatus.h"
#else
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#endif
#include "xhls_ip_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Control_BaseAddress;
} XHls_ip_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XHls_ip;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XHls_ip_WriteReg(BaseAddress, RegOffset, Data)                                             \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XHls_ip_ReadReg(BaseAddress, RegOffset) Xil_In32((BaseAddress) + (RegOffset))
#else
#define XHls_ip_WriteReg(BaseAddress, RegOffset, Data)                                             \
    *(volatile u32 *)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XHls_ip_ReadReg(BaseAddress, RegOffset) *(volatile u32 *)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr) assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS 0
#define XST_DEVICE_NOT_FOUND 2
#define XST_OPEN_DEVICE_FAILED 3
#define XIL_COMPONENT_IS_READY 1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XHls_ip_Initialize(XHls_ip *InstancePtr, UINTPTR BaseAddress);
XHls_ip_Config *XHls_ip_LookupConfig(UINTPTR BaseAddress);
#else
int XHls_ip_Initialize(XHls_ip *InstancePtr, u16 DeviceId);
XHls_ip_Config *XHls_ip_LookupConfig(u16 DeviceId);
#endif
int XHls_ip_CfgInitialize(XHls_ip *InstancePtr, XHls_ip_Config *ConfigPtr);
#else
int XHls_ip_Initialize(XHls_ip *InstancePtr, const char *InstanceName);
int XHls_ip_Release(XHls_ip *InstancePtr);
#endif

void XHls_ip_Start(XHls_ip *InstancePtr);
u32 XHls_ip_IsDone(XHls_ip *InstancePtr);
u32 XHls_ip_IsIdle(XHls_ip *InstancePtr);
u32 XHls_ip_IsReady(XHls_ip *InstancePtr);
void XHls_ip_EnableAutoRestart(XHls_ip *InstancePtr);
void XHls_ip_DisableAutoRestart(XHls_ip *InstancePtr);
u32 XHls_ip_Get_return(XHls_ip *InstancePtr);
void Xhls_ip_Set_data(XHls_ip *InstancePtr, int size, char* data);
void Xhls_ip_Get_data(XHls_ip *InstancePtr, int size, char* data);
void XHls_ip_InterruptGlobalEnable(XHls_ip *InstancePtr);
void XHls_ip_InterruptGlobalDisable(XHls_ip *InstancePtr);
void XHls_ip_InterruptEnable(XHls_ip *InstancePtr, u32 Mask);
void XHls_ip_InterruptDisable(XHls_ip *InstancePtr, u32 Mask);
void XHls_ip_InterruptClear(XHls_ip *InstancePtr, u32 Mask);
u32 XHls_ip_InterruptGetEnabled(XHls_ip *InstancePtr);
u32 XHls_ip_InterruptGetStatus(XHls_ip *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif