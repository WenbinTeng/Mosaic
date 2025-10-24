// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2023.2 (64-bit)
// Tool Version Limit: 2023.10
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
//
// ==============================================================
/***************************** Include Files *********************************/
#include "xhls_ip.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XHls_ip_CfgInitialize(XHls_ip *InstancePtr, XHls_ip_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XHls_ip_Start(XHls_ip *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL) & 0x80;
    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XHls_ip_IsDone(XHls_ip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XHls_ip_IsIdle(XHls_ip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XHls_ip_IsReady(XHls_ip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XHls_ip_EnableAutoRestart(XHls_ip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XHls_ip_DisableAutoRestart(XHls_ip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_CTRL, 0);
}

u32 XHls_ip_Get_return(XHls_ip *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_AP_RETURN);
    return Data;
}

void Xhls_ip_Set_data(XHls_ip *InstancePtr, int size, char* data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    for (int i = 0; i < size; i++) {
        XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_INPUT_DATA + i, data[i]);
    }
}

void Xhls_ip_Get_data(XHls_ip *InstancePtr, int size, char* data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    for (int i = 0; i < size; i++) {
        data[i] = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_INPUT_DATA + i);
    }
} 

void XHls_ip_InterruptGlobalEnable(XHls_ip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_GIE, 1);
}

void XHls_ip_InterruptGlobalDisable(XHls_ip *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_GIE, 0);
}

void XHls_ip_InterruptEnable(XHls_ip *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_IER);
    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_IER, Register | Mask);
}

void XHls_ip_InterruptDisable(XHls_ip *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register = XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_IER);
    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_IER,
                     Register & (~Mask));
}

void XHls_ip_InterruptClear(XHls_ip *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XHls_ip_WriteReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_ISR, Mask);
}

u32 XHls_ip_InterruptGetEnabled(XHls_ip *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_IER);
}

u32 XHls_ip_InterruptGetStatus(XHls_ip *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XHls_ip_ReadReg(InstancePtr->Control_BaseAddress, XHLS_IP_CONTROL_ADDR_ISR);
}
