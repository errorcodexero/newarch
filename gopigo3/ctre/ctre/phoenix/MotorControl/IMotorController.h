#pragma once

#include "NeutralMode.h"
#include "InvertType.h"
#include "ControlMode.h"
#include <frc/GPG3MotorController.h>
#include <cassert>


namespace ctre
{
    namespace phoenix
    {
        namespace motorcontrol
        {
            class IMotorController : public frc::GPG3MotorController
            {
            public:
                IMotorController(int id)  : frc::GPG3MotorController(id)  {
                    isInverted_ = false;
                }

                virtual ~IMotorController() {
                }

                void Follow(IMotorController &talon) {      
                }                

                void SetNeutralMode(NeutralMode mode) {                    
                }

                void SetInverted(InvertType type) {                    
                }

                void SetInverted(bool b) {
                    isInverted_ = b;
                }

                void Set(ControlMode mode, double value) {
                    double motorPower = value;
                    assert(mode == ControlMode::PercentOutput) ;
                    if (isInverted_) {
                        motorPower *= -1;
                    }
                    frc::GPG3MotorController::Set(motorPower) ;
                }

                void ConfigVoltageCompSaturation(double voltage, int timeout) {
                }

                void EnableVoltageCompensation(bool b) {                    
                }

            private:
                bool isInverted_;
            } ;
        }
    }
}