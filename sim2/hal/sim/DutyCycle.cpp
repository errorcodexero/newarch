/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "hal/DutyCycle.h"

#include "HALInitializer.h"
#include "PortsInternal.h"
#include "hal/Errors.h"
#include "hal/handles/HandlesInternal.h"
#include "hal/handles/LimitedHandleResource.h"
#include "mockdata/DutyCycleDataInternal.h"

using namespace hal;

namespace {
struct DutyCycle {
  uint8_t index;
};
struct Empty {};
}  // namespace

static LimitedHandleResource<HAL_DutyCycleHandle, DutyCycle, kNumDutyCycles,
                             HAL_HandleEnum::DutyCycle>* dutyCycleHandles;

namespace hal {
namespace init {
void InitializeDutyCycle() {
  static LimitedHandleResource<HAL_DutyCycleHandle, DutyCycle, kNumDutyCycles,
                               HAL_HandleEnum::DutyCycle>
      dcH;
  dutyCycleHandles = &dcH;
}
}  // namespace init
}  // namespace hal

extern "C" {
HAL_DutyCycleHandle HAL_InitializeDutyCycle(HAL_Handle digitalSourceHandle,
                                            HAL_AnalogTriggerType triggerType,
                                            int32_t* status) {
  hal::init::CheckInit();

  HAL_DutyCycleHandle handle = dutyCycleHandles->Allocate();
  if (handle == HAL_kInvalidHandle) {
    *status = NO_AVAILABLE_RESOURCES;
    return HAL_kInvalidHandle;
  }

  auto dutyCycle = dutyCycleHandles->Get(handle);
  if (dutyCycle == nullptr) {  // would only occur on thread issue
    *status = HAL_HANDLE_ERROR;
    return HAL_kInvalidHandle;
  }

  int16_t index = getHandleIndex(handle);
  SimDutyCycleData[index].digitalChannel = getHandleIndex(digitalSourceHandle);
  SimDutyCycleData[index].initialized = true;
  SimDutyCycleData[index].simDevice = 0;
  dutyCycle->index = index;
  return handle;
}
void HAL_FreeDutyCycle(HAL_DutyCycleHandle dutyCycleHandle) {
  auto dutyCycle = dutyCycleHandles->Get(dutyCycleHandle);
  dutyCycleHandles->Free(dutyCycleHandle);
  if (dutyCycle == nullptr) return;
  SimDutyCycleData[dutyCycle->index].initialized = false;
}

void HAL_SetDutyCycleSimDevice(HAL_EncoderHandle handle,
                               HAL_SimDeviceHandle device) {
  auto dutyCycle = dutyCycleHandles->Get(handle);
  if (dutyCycle == nullptr) return;
  SimDutyCycleData[dutyCycle->index].simDevice = device;
}

int32_t HAL_GetDutyCycleFrequency(HAL_DutyCycleHandle dutyCycleHandle,
                                  int32_t* status) {
  auto dutyCycle = dutyCycleHandles->Get(dutyCycleHandle);
  if (dutyCycle == nullptr) {
    *status = HAL_HANDLE_ERROR;
    return 0;
  }
  return SimDutyCycleData[dutyCycle->index].frequency;
}
double HAL_GetDutyCycleOutput(HAL_DutyCycleHandle dutyCycleHandle,
                              int32_t* status) {
  auto dutyCycle = dutyCycleHandles->Get(dutyCycleHandle);
  if (dutyCycle == nullptr) {
    *status = HAL_HANDLE_ERROR;
    return 0;
  }
  return SimDutyCycleData[dutyCycle->index].output;
}
int32_t HAL_GetDutyCycleOutputRaw(HAL_DutyCycleHandle dutyCycleHandle,
                                  int32_t* status) {
  auto dutyCycle = dutyCycleHandles->Get(dutyCycleHandle);
  if (dutyCycle == nullptr) {
    *status = HAL_HANDLE_ERROR;
    return 0;
  }
  return SimDutyCycleData[dutyCycle->index].output *
         HAL_GetDutyCycleOutputScaleFactor(dutyCycleHandle, status);
}
int32_t HAL_GetDutyCycleOutputScaleFactor(HAL_DutyCycleHandle dutyCycleHandle,
                                          int32_t* status) {
  return 4e7 - 1;
}
int32_t HAL_GetDutyCycleFPGAIndex(HAL_DutyCycleHandle dutyCycleHandle,
                                  int32_t* status) {
  auto dutyCycle = dutyCycleHandles->Get(dutyCycleHandle);
  if (dutyCycle == nullptr) {
    *status = HAL_HANDLE_ERROR;
    return -1;
  }
  return dutyCycle->index;
}
}  // extern "C"
