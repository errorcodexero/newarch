/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "NotifyListener.h"
#include "hal/Types.h"

#ifdef __cplusplus
extern "C" {
#endif

void HALSIM_ResetAnalogOutData(int32_t index);
int32_t HALSIM_RegisterAnalogOutVoltageCallback(int32_t index,
                                                HAL_NotifyCallback callback,
                                                void* param,
                                                HAL_Bool initialNotify);
void HALSIM_CancelAnalogOutVoltageCallback(int32_t index, int32_t uid);
double HALSIM_GetAnalogOutVoltage(int32_t index);
void HALSIM_SetAnalogOutVoltage(int32_t index, double voltage);

int32_t HALSIM_RegisterAnalogOutInitializedCallback(int32_t index,
                                                    HAL_NotifyCallback callback,
                                                    void* param,
                                                    HAL_Bool initialNotify);
void HALSIM_CancelAnalogOutInitializedCallback(int32_t index, int32_t uid);
HAL_Bool HALSIM_GetAnalogOutInitialized(int32_t index);
void HALSIM_SetAnalogOutInitialized(int32_t index, HAL_Bool initialized);

void HALSIM_RegisterAnalogOutAllCallbacks(int32_t index,
                                          HAL_NotifyCallback callback,
                                          void* param, HAL_Bool initialNotify);

#ifdef __cplusplus
}  // extern "C"
#endif
