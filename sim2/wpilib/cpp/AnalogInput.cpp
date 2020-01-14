/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/AnalogInput.h"

#include <utility>

#include <hal/AnalogAccumulator.h>
#include <hal/AnalogInput.h>
#include <hal/FRCUsageReporting.h>
#include <hal/HALBase.h>
#include <hal/Ports.h>

#include "frc/SensorUtil.h"
#include "frc/Timer.h"
#include "frc/WPIErrors.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"

using namespace frc;

AnalogInput::AnalogInput(int channel) {
  if (!SensorUtil::CheckAnalogInputChannel(channel)) {
    wpi_setWPIErrorWithContext(ChannelIndexOutOfRange,
                               "Analog Input " + wpi::Twine(channel));
    return;
  }

  m_channel = channel;

  HAL_PortHandle port = HAL_GetPort(channel);
  int32_t status = 0;
  m_port = HAL_InitializeAnalogInputPort(port, &status);
  if (status != 0) {
    wpi_setHALErrorWithRange(status, 0, HAL_GetNumAnalogInputs(), channel);
    m_channel = std::numeric_limits<int>::max();
    m_port = HAL_kInvalidHandle;
    return;
  }

  HAL_Report(HALUsageReporting::kResourceType_AnalogChannel, channel + 1);

  SendableRegistry::GetInstance().AddLW(this, "AnalogInput", channel);
}

AnalogInput::~AnalogInput() { HAL_FreeAnalogInputPort(m_port); }

int AnalogInput::GetValue() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int value = HAL_GetAnalogValue(m_port, &status);
  wpi_setHALError(status);
  return value;
}

int AnalogInput::GetAverageValue() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int value = HAL_GetAnalogAverageValue(m_port, &status);
  wpi_setHALError(status);
  return value;
}

double AnalogInput::GetVoltage() const {
  if (StatusIsFatal()) return 0.0;
  int32_t status = 0;
  double voltage = HAL_GetAnalogVoltage(m_port, &status);
  wpi_setHALError(status);
  return voltage;
}

double AnalogInput::GetAverageVoltage() const {
  if (StatusIsFatal()) return 0.0;
  int32_t status = 0;
  double voltage = HAL_GetAnalogAverageVoltage(m_port, &status);
  wpi_setHALError(status);
  return voltage;
}

int AnalogInput::GetChannel() const {
  if (StatusIsFatal()) return 0;
  return m_channel;
}

void AnalogInput::SetAverageBits(int bits) {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  HAL_SetAnalogAverageBits(m_port, bits, &status);
  wpi_setHALError(status);
}

int AnalogInput::GetAverageBits() const {
  int32_t status = 0;
  int averageBits = HAL_GetAnalogAverageBits(m_port, &status);
  wpi_setHALError(status);
  return averageBits;
}

void AnalogInput::SetOversampleBits(int bits) {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  HAL_SetAnalogOversampleBits(m_port, bits, &status);
  wpi_setHALError(status);
}

int AnalogInput::GetOversampleBits() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int oversampleBits = HAL_GetAnalogOversampleBits(m_port, &status);
  wpi_setHALError(status);
  return oversampleBits;
}

int AnalogInput::GetLSBWeight() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int lsbWeight = HAL_GetAnalogLSBWeight(m_port, &status);
  wpi_setHALError(status);
  return lsbWeight;
}

int AnalogInput::GetOffset() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int offset = HAL_GetAnalogOffset(m_port, &status);
  wpi_setHALError(status);
  return offset;
}

bool AnalogInput::IsAccumulatorChannel() const {
  if (StatusIsFatal()) return false;
  int32_t status = 0;
  bool isAccum = HAL_IsAccumulatorChannel(m_port, &status);
  wpi_setHALError(status);
  return isAccum;
}

void AnalogInput::InitAccumulator() {
  if (StatusIsFatal()) return;
  m_accumulatorOffset = 0;
  int32_t status = 0;
  HAL_InitAccumulator(m_port, &status);
  wpi_setHALError(status);
}

void AnalogInput::SetAccumulatorInitialValue(int64_t initialValue) {
  if (StatusIsFatal()) return;
  m_accumulatorOffset = initialValue;
}

void AnalogInput::ResetAccumulator() {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  HAL_ResetAccumulator(m_port, &status);
  wpi_setHALError(status);

  if (!StatusIsFatal()) {
    // Wait until the next sample, so the next call to GetAccumulator*()
    // won't have old values.
    const double sampleTime = 1.0 / GetSampleRate();
    const double overSamples = 1 << GetOversampleBits();
    const double averageSamples = 1 << GetAverageBits();
    Wait(sampleTime * overSamples * averageSamples);
  }
}

void AnalogInput::SetAccumulatorCenter(int center) {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  HAL_SetAccumulatorCenter(m_port, center, &status);
  wpi_setHALError(status);
}

void AnalogInput::SetAccumulatorDeadband(int deadband) {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  HAL_SetAccumulatorDeadband(m_port, deadband, &status);
  wpi_setHALError(status);
}

int64_t AnalogInput::GetAccumulatorValue() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int64_t value = HAL_GetAccumulatorValue(m_port, &status);
  wpi_setHALError(status);
  return value + m_accumulatorOffset;
}

int64_t AnalogInput::GetAccumulatorCount() const {
  if (StatusIsFatal()) return 0;
  int32_t status = 0;
  int64_t count = HAL_GetAccumulatorCount(m_port, &status);
  wpi_setHALError(status);
  return count;
}

void AnalogInput::GetAccumulatorOutput(int64_t& value, int64_t& count) const {
  if (StatusIsFatal()) return;
  int32_t status = 0;
  HAL_GetAccumulatorOutput(m_port, &value, &count, &status);
  wpi_setHALError(status);
  value += m_accumulatorOffset;
}

void AnalogInput::SetSampleRate(double samplesPerSecond) {
  int32_t status = 0;
  HAL_SetAnalogSampleRate(samplesPerSecond, &status);
  wpi_setGlobalHALError(status);
}

double AnalogInput::GetSampleRate() {
  int32_t status = 0;
  double sampleRate = HAL_GetAnalogSampleRate(&status);
  wpi_setGlobalHALError(status);
  return sampleRate;
}

double AnalogInput::PIDGet() {
  if (StatusIsFatal()) return 0.0;
  return GetAverageVoltage();
}

void AnalogInput::SetSimDevice(HAL_SimDeviceHandle device) {
  HAL_SetAnalogInputSimDevice(m_port, device);
}

void AnalogInput::InitSendable(SendableBuilder& builder) {
  builder.SetSmartDashboardType("Analog Input");
  builder.AddDoubleProperty("Value", [=]() { return GetAverageVoltage(); },
                            nullptr);
}
