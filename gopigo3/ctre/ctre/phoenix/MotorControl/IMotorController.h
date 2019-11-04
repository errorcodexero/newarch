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
                }

                void Set(ControlMode mode, double value) {
                    assert(mode == ControlMode::PercentOutput) ;
                    frc::GPG3MotorController::Set(value) ;
                }

                void ConfigVoltageCompSaturation(double voltage, int timeout) {
                }

                void EnableVoltageCompensation(bool b) {                    
                }

            private:
            } ;
        }
    }
}