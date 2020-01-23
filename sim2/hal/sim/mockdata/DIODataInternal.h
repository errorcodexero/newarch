/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "mockdata/DIOData.h"
#include "mockdata/SimDataValue.h"

namespace hal {
class DIOData {
  HAL_SIMDATAVALUE_DEFINE_NAME(Initialized)
  HAL_SIMDATAVALUE_DEFINE_NAME(Value)
  HAL_SIMDATAVALUE_DEFINE_NAME(PulseLength)
  HAL_SIMDATAVALUE_DEFINE_NAME(IsInput)
  HAL_SIMDATAVALUE_DEFINE_NAME(FilterIndex)

 public:
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetInitializedName> initialized{
      false};
  std::atomic<HAL_SimDeviceHandle> simDevice;
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetValueName> value{true};
  SimDataValue<double, HAL_MakeDouble, GetPulseLengthName> pulseLength{0.0};
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetIsInputName> isInput{true};
  SimDataValue<int32_t, HAL_MakeInt, GetFilterIndexName> filterIndex{-1};

  virtual void ResetData();
};
extern DIOData* SimDIOData;
}  // namespace hal
