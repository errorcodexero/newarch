/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "frc/drive/DifferentialDrive.h"

#include <algorithm>
#include <cmath>

#include <hal/FRCUsageReporting.h>

#include "frc/SpeedController.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"

using namespace frc;

DifferentialDrive::DifferentialDrive(SpeedController& leftMotor,
                                     SpeedController& rightMotor)
    : m_leftMotor(&leftMotor), m_rightMotor(&rightMotor) {
  auto& registry = SendableRegistry::GetInstance();
  registry.AddChild(this, m_leftMotor);
  registry.AddChild(this, m_rightMotor);
  static int instances = 0;
  ++instances;
  registry.AddLW(this, "DifferentialDrive", instances);
}

void DifferentialDrive::ArcadeDrive(double xSpeed, double zRotation,
                                    bool squareInputs) {
  static bool reported = false;
  if (!reported) {
    HAL_Report(HALUsageReporting::kResourceType_RobotDrive,
               HALUsageReporting::kRobotDrive2_DifferentialArcade, 2);
    reported = true;
  }

  xSpeed = std::clamp(xSpeed, -1.0, 1.0);
  xSpeed = ApplyDeadband(xSpeed, m_deadband);

  zRotation = std::clamp(zRotation, -1.0, 1.0);
  zRotation = ApplyDeadband(zRotation, m_deadband);

  // Square the inputs (while preserving the sign) to increase fine control
  // while permitting full power.
  if (squareInputs) {
    xSpeed = std::copysign(xSpeed * xSpeed, xSpeed);
    zRotation = std::copysign(zRotation * zRotation, zRotation);
  }

  double leftMotorOutput;
  double rightMotorOutput;

  double maxInput =
      std::copysign(std::max(std::abs(xSpeed), std::abs(zRotation)), xSpeed);

  if (xSpeed >= 0.0) {
    // First quadrant, else second quadrant
    if (zRotation >= 0.0) {
      leftMotorOutput = maxInput;
      rightMotorOutput = xSpeed - zRotation;
    } else {
      leftMotorOutput = xSpeed + zRotation;
      rightMotorOutput = maxInput;
    }
  } else {
    // Third quadrant, else fourth quadrant
    if (zRotation >= 0.0) {
      leftMotorOutput = xSpeed + zRotation;
      rightMotorOutput = maxInput;
    } else {
      leftMotorOutput = maxInput;
      rightMotorOutput = xSpeed - zRotation;
    }
  }

  m_leftMotor->Set(std::clamp(leftMotorOutput, -1.0, 1.0) * m_maxOutput);
  double maxOutput = m_maxOutput * m_rightSideInvertMultiplier;
  m_rightMotor->Set(std::clamp(rightMotorOutput, -1.0, 1.0) * maxOutput);

  Feed();
}

void DifferentialDrive::CurvatureDrive(double xSpeed, double zRotation,
                                       bool isQuickTurn) {
  static bool reported = false;
  if (!reported) {
    HAL_Report(HALUsageReporting::kResourceType_RobotDrive,
               HALUsageReporting::kRobotDrive2_DifferentialCurvature, 2);
    reported = true;
  }

  xSpeed = std::clamp(xSpeed, -1.0, 1.0);
  xSpeed = ApplyDeadband(xSpeed, m_deadband);

  zRotation = std::clamp(zRotation, -1.0, 1.0);
  zRotation = ApplyDeadband(zRotation, m_deadband);

  double angularPower;
  bool overPower;

  if (isQuickTurn) {
    if (std::abs(xSpeed) < m_quickStopThreshold) {
      m_quickStopAccumulator =
          (1 - m_quickStopAlpha) * m_quickStopAccumulator +
          m_quickStopAlpha * std::clamp(zRotation, -1.0, 1.0) * 2;
    }
    overPower = true;
    angularPower = zRotation;
  } else {
    overPower = false;
    angularPower = std::abs(xSpeed) * zRotation - m_quickStopAccumulator;

    if (m_quickStopAccumulator > 1) {
      m_quickStopAccumulator -= 1;
    } else if (m_quickStopAccumulator < -1) {
      m_quickStopAccumulator += 1;
    } else {
      m_quickStopAccumulator = 0.0;
    }
  }

  double leftMotorOutput = xSpeed + angularPower;
  double rightMotorOutput = xSpeed - angularPower;

  // If rotation is overpowered, reduce both outputs to within acceptable range
  if (overPower) {
    if (leftMotorOutput > 1.0) {
      rightMotorOutput -= leftMotorOutput - 1.0;
      leftMotorOutput = 1.0;
    } else if (rightMotorOutput > 1.0) {
      leftMotorOutput -= rightMotorOutput - 1.0;
      rightMotorOutput = 1.0;
    } else if (leftMotorOutput < -1.0) {
      rightMotorOutput -= leftMotorOutput + 1.0;
      leftMotorOutput = -1.0;
    } else if (rightMotorOutput < -1.0) {
      leftMotorOutput -= rightMotorOutput + 1.0;
      rightMotorOutput = -1.0;
    }
  }

  // Normalize the wheel speeds
  double maxMagnitude =
      std::max(std::abs(leftMotorOutput), std::abs(rightMotorOutput));
  if (maxMagnitude > 1.0) {
    leftMotorOutput /= maxMagnitude;
    rightMotorOutput /= maxMagnitude;
  }

  m_leftMotor->Set(leftMotorOutput * m_maxOutput);
  m_rightMotor->Set(rightMotorOutput * m_maxOutput *
                    m_rightSideInvertMultiplier);

  Feed();
}

void DifferentialDrive::TankDrive(double leftSpeed, double rightSpeed,
                                  bool squareInputs) {
  static bool reported = false;
  if (!reported) {
    HAL_Report(HALUsageReporting::kResourceType_RobotDrive,
               HALUsageReporting::kRobotDrive2_DifferentialTank, 2);
    reported = true;
  }

  leftSpeed = std::clamp(leftSpeed, -1.0, 1.0);
  leftSpeed = ApplyDeadband(leftSpeed, m_deadband);

  rightSpeed = std::clamp(rightSpeed, -1.0, 1.0);
  rightSpeed = ApplyDeadband(rightSpeed, m_deadband);

  // Square the inputs (while preserving the sign) to increase fine control
  // while permitting full power.
  if (squareInputs) {
    leftSpeed = std::copysign(leftSpeed * leftSpeed, leftSpeed);
    rightSpeed = std::copysign(rightSpeed * rightSpeed, rightSpeed);
  }

  m_leftMotor->Set(leftSpeed * m_maxOutput);
  m_rightMotor->Set(rightSpeed * m_maxOutput * m_rightSideInvertMultiplier);

  Feed();
}

void DifferentialDrive::SetQuickStopThreshold(double threshold) {
  m_quickStopThreshold = threshold;
}

void DifferentialDrive::SetQuickStopAlpha(double alpha) {
  m_quickStopAlpha = alpha;
}

bool DifferentialDrive::IsRightSideInverted() const {
  return m_rightSideInvertMultiplier == -1.0;
}

void DifferentialDrive::SetRightSideInverted(bool rightSideInverted) {
  m_rightSideInvertMultiplier = rightSideInverted ? -1.0 : 1.0;
}

void DifferentialDrive::StopMotor() {
  m_leftMotor->StopMotor();
  m_rightMotor->StopMotor();
  Feed();
}

void DifferentialDrive::GetDescription(wpi::raw_ostream& desc) const {
  desc << "DifferentialDrive";
}

void DifferentialDrive::InitSendable(SendableBuilder& builder) {
  builder.SetSmartDashboardType("DifferentialDrive");
  builder.SetActuator(true);
  builder.SetSafeState([=] { StopMotor(); });
  builder.AddDoubleProperty("Left Motor Speed",
                            [=]() { return m_leftMotor->Get(); },
                            [=](double value) { m_leftMotor->Set(value); });
  builder.AddDoubleProperty(
      "Right Motor Speed",
      [=]() { return m_rightMotor->Get() * m_rightSideInvertMultiplier; },
      [=](double value) {
        m_rightMotor->Set(value * m_rightSideInvertMultiplier);
      });
}
