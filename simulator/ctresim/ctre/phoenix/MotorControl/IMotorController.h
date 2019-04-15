#pragma once

#include <ctre/phoenix/MotorControl/SensorCollection.h>
#include <ctre/phoenix/MotorControl/ControlMode.h>
#include <ctre/phoenix/MotorControl/NeutralMode.h>
#include <ctre/phoenix/MotorControl/LimitSwitchType.h>
#include <frc/SimulatedObject.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {

            class IMotorController : public xero::sim::SimulatedObject
            {
            public:
                IMotorController(int index) ;
                virtual ~IMotorController() ;
                
                virtual void Set(ctre::phoenix::motorcontrol::ControlMode mode, double v);
                virtual double Get() {
                    return value_ ;
                }

                int GetDeviceID() {
                    return index_ ;
                }

                void SetInverted(bool inverted) {
                    inverted_ = inverted ;
                }

                ctre::phoenix::motorcontrol::SensorCollection& GetSensorCollection() ;

                void Follow(IMotorController& master) {                    
                }

                void ConfigVoltageCompSaturation(double v, int timeout) {
                }

                void EnableVoltageCompensation(bool b) {
                }

                void SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode mode) {
                }        

                void ConfigForwardLimitSwitchSource(LimitSwitchSource source, LimitSwitchNormal state, int to = 0) {
                }    

                void ConfigReverseLimitSwitchSource(LimitSwitchSource source, LimitSwitchNormal state, int to = 0) {
                }           

                void ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource type, LimitSwitchNormal state, int deviceID, int to = 0) {
                }       

                void ConfigReverseLimitSwitchSource(RemoteLimitSwitchSource type, LimitSwitchNormal state, int deviceID, int to = 0) {                    
                }
                
            private:
                int index_ ;
                double value_ ;
                bool inverted_ ;
                ctre::phoenix::motorcontrol::SensorCollection sensors_ ;            
            } ;
        }
    }
}