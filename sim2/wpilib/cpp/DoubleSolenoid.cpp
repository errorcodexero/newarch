/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/DoubleSolenoid.h"

#include <utility>

#include <hal/FRCUsageReporting.h>
#include <hal/HALBase.h>
#include <hal/Ports.h>
#include <hal/Solenoid.h>

#include "frc/SensorUtil.h"
#include "frc/WPIErrors.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"

using namespace frc;

DoubleSolenoid::DoubleSolenoid(int forwardChannel, int reverseChannel)
    : DoubleSolenoid(SensorUtil::GetDefaultSolenoidModule(), forwardChannel,
                     reverseChannel) {}

DoubleSolenoid::DoubleSolenoid(int moduleNumber, int forwardChannel,
                               int reverseChannel)
    : SolenoidBase(moduleNumber),
      m_forwardChannel(forwardChannel),
      m_reverseChannel(reverseChannel) {
  if (!SensorUtil::CheckSolenoidModule(m_moduleNumber)) {
    wpi_setWPIErrorWithContext(ModuleIndexOutOfRange,
                               "Solenoid Module " + wpi::Twine(m_moduleNumber));
    return;
  }
  if (!SensorUtil::CheckSolenoidChannel(m_forwardChannel)) {
    wpi_setWPIErrorWithContext(
        ChannelIndexOutOfRange,
        "Solenoid Channel " + wpi::Twine(m_forwardChannel));
    return;
  }
  if (!SensorUtil::CheckSolenoidChannel(m_reverseChannel)) {
    wpi_setWPIErrorWithContext(
        ChannelIndexOutOfRange,
        "Solenoid Channel " + wpi::Twine(m_reverseChannel));
    return;
  }
  int32_t status = 0;
  m_forwardHandle = HAL_InitializeSolenoidPort(
      HAL_GetPortWithModule(moduleNumber, m_forwardChannel), &status);
  if (status != 0) {
    wpi_setHALErrorWithRange(status, 0, HAL_GetNumSolenoidChannels(),
                             forwardChannel);
    m_forwardHandle = HAL_kInvalidHandle;
    m_reverseHandle = HAL_kInvalidHandle;
    return;
  }

  m_reverseHandle = HAL_InitializeSolenoidPort(
      HAL_GetPortWithModule(moduleNumber, m_reverseChannel), &status);
  if (status != 0) {
    wpi_setHALErrorWithRange(status, 0, HAL_GetNumSolenoidChannels(),
                             reverseChannel);
    // free forward solenoid
    HAL_FreeSolenoidPort(m_forwardHandle);
    m_forwardHandle = HAL_kInvalidHandle;
    m_reverseHandle = HAL_kInvalidHandle;
    return;
  }

  m_forwardMask = 1 << m_forwardChannel;
  m_reverseMask = 1 << m_reverseChannel;

  HAL_Report(HALUsageReporting::kResourceType_Solenoid, m_forwardChannel + 1,
             m_moduleNumber + 1);
  HAL_Report(HALUsageReporting::kResourceType_Solenoid, m_reverseChannel + 1,
             m_moduleNumber + 1);

  SendableRegistry::GetInstance().AddLW(this, "DoubleSolenoid", m_moduleNumber,
                                        m_forwardChannel);
}

DoubleSolenoid::~DoubleSolenoid() {
  HAL_FreeSolenoidPort(m_forwardHandle);
  HAL_FreeSolenoidPort(m_reverseHandle);
}

void DoubleSolenoid::Set(Value value) {
  if (StatusIsFatal()) return;

  bool forward = false;
  bool reverse = false;
  switch (value) {
    case kOff:
      forward = false;
      reverse = false;
      break;
    case kForward:
      forward = true;
      reverse = false;
      break;
    case kReverse:
      forward = false;
      reverse = true;
      break;
  }
  int fstatus = 0;
  HAL_SetSolenoid(m_forwardHandle, forward, &fstatus);
  int rstatus = 0;
  HAL_SetSolenoid(m_reverseHandle, reverse, &rstatus);

  wpi_setHALError(fstatus);
  wpi_setHALError(rstatus);
}

DoubleSolenoid::Value DoubleSolenoid::Get() const {
  if (StatusIsFatal()) return kOff;
  int fstatus = 0;
  int rstatus = 0;
  bool valueForward = HAL_GetSolenoid(m_forwardHandle, &fstatus);
  bool valueReverse = HAL_GetSolenoid(m_reverseHandle, &rstatus);

  wpi_setHALError(fstatus);
  wpi_setHALError(rstatus);

  if (valueForward) return kForward;
  if (valueReverse) return kReverse;
  return kOff;
}

bool DoubleSolenoid::IsFwdSolenoidBlackListed() const {
  int blackList = GetPCMSolenoidBlackList(m_moduleNumber);
  return (blackList & m_forwardMask) != 0;
}

bool DoubleSolenoid::IsRevSolenoidBlackListed() const {
  int blackList = GetPCMSolenoidBlackList(m_moduleNumber);
  return (blackList & m_reverseMask) != 0;
}

void DoubleSolenoid::InitSendable(SendableBuilder& builder) {
  builder.SetSmartDashboardType("Double Solenoid");
  builder.SetActuator(true);
  builder.SetSafeState([=]() { Set(kOff); });
  builder.AddSmallStringProperty(
      "Value",
      [=](wpi::SmallVectorImpl<char>& buf) -> wpi::StringRef {
        switch (Get()) {
          case kForward:
            return "Forward";
          case kReverse:
            return "Reverse";
          default:
            return "Off";
        }
      },
      [=](wpi::StringRef value) {
        Value lvalue = kOff;
        if (value == "Forward")
          lvalue = kForward;
        else if (value == "Reverse")
          lvalue = kReverse;
        Set(lvalue);
      });
}
