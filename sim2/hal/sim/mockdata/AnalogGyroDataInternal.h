/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "mockdata/AnalogGyroData.h"
#include "mockdata/SimDataValue.h"

namespace hal {
class AnalogGyroData {
  HAL_SIMDATAVALUE_DEFINE_NAME(Angle)
  HAL_SIMDATAVALUE_DEFINE_NAME(Rate)
  HAL_SIMDATAVALUE_DEFINE_NAME(Initialized)

 public:
  SimDataValue<double, HAL_MakeDouble, GetAngleName> angle{0.0};
  SimDataValue<double, HAL_MakeDouble, GetRateName> rate{0.0};
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetInitializedName> initialized{
      false};

  virtual void ResetData();
};
extern AnalogGyroData* SimAnalogGyroData;
}  // namespace hal
