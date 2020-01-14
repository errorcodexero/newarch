/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/AnalogTriggerOutput.h"

#include <hal/FRCUsageReporting.h>

#include "frc/AnalogTrigger.h"
#include "frc/WPIErrors.h"

using namespace frc;

bool AnalogTriggerOutput::Get() const {
  int32_t status = 0;
  bool result = HAL_GetAnalogTriggerOutput(
      m_trigger->m_trigger, static_cast<HAL_AnalogTriggerType>(m_outputType),
      &status);
  wpi_setHALError(status);
  return result;
}

HAL_Handle AnalogTriggerOutput::GetPortHandleForRouting() const {
  return m_trigger->m_trigger;
}

AnalogTriggerType AnalogTriggerOutput::GetAnalogTriggerTypeForRouting() const {
  return m_outputType;
}

bool AnalogTriggerOutput::IsAnalogTrigger() const { return true; }

int AnalogTriggerOutput::GetChannel() const { return m_trigger->GetIndex(); }

void AnalogTriggerOutput::InitSendable(SendableBuilder&) {}

AnalogTriggerOutput::AnalogTriggerOutput(const AnalogTrigger& trigger,
                                         AnalogTriggerType outputType)
    : m_trigger(&trigger), m_outputType(outputType) {
  HAL_Report(HALUsageReporting::kResourceType_AnalogTriggerOutput,
             trigger.GetIndex() + 1, static_cast<uint8_t>(outputType) + 1);
}
