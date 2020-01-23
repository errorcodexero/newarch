/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "../PortsInternal.h"
#include "mockdata/PCMData.h"
#include "mockdata/SimDataValue.h"

namespace hal {
class PCMData {
  HAL_SIMDATAVALUE_DEFINE_NAME(SolenoidInitialized)
  HAL_SIMDATAVALUE_DEFINE_NAME(SolenoidOutput)
  HAL_SIMDATAVALUE_DEFINE_NAME(CompressorInitialized)
  HAL_SIMDATAVALUE_DEFINE_NAME(CompressorOn)
  HAL_SIMDATAVALUE_DEFINE_NAME(ClosedLoopEnabled)
  HAL_SIMDATAVALUE_DEFINE_NAME(PressureSwitch)
  HAL_SIMDATAVALUE_DEFINE_NAME(CompressorCurrent)

  static LLVM_ATTRIBUTE_ALWAYS_INLINE constexpr HAL_Bool
  GetSolenoidInitializedDefault() {
    return false;
  }
  static LLVM_ATTRIBUTE_ALWAYS_INLINE constexpr HAL_Bool
  GetSolenoidOutputDefault() {
    return false;
  }

 public:
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetSolenoidInitializedName,
               GetSolenoidInitializedDefault>
      solenoidInitialized[kNumSolenoidChannels];
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetSolenoidOutputName,
               GetSolenoidOutputDefault>
      solenoidOutput[kNumSolenoidChannels];
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetCompressorInitializedName>
      compressorInitialized{false};
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetCompressorOnName> compressorOn{
      false};
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetClosedLoopEnabledName>
      closedLoopEnabled{true};
  SimDataValue<HAL_Bool, HAL_MakeBoolean, GetPressureSwitchName> pressureSwitch{
      false};
  SimDataValue<double, HAL_MakeDouble, GetCompressorCurrentName>
      compressorCurrent{0.0};

  virtual void ResetData();
};
extern PCMData* SimPCMData;
}  // namespace hal
