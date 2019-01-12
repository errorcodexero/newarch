#pragma once

#include <frc/SimulatedObject.h>
#include <ctre/phoenix/MotorControl/SensorCollection.h>
#include <ctre/phoenix/MotorControl/ControlMode.h>
#include <ctre/phoenix/MotorControl/NeutralMode.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {
                class TalonSRX : public xero::sim::SimulatedObject
                {
                    friend xero::sim::RobotSimBase ;
                public:
                    TalonSRX(int index);
                    virtual ~TalonSRX();

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

                    void Follow(TalonSRX& master) {};

                    void ConfigVoltageCompSaturation(double v, int timeout) {
                    }

                    void EnableVoltageCompensation(bool b) {
                    }

                    void SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode mode) {
                    }

                private:
                    int index_ ;
                    double value_ ;
                    bool inverted_ ;
                    ctre::phoenix::motorcontrol::SensorCollection sensors_ ;
                };
            }
        }
    }
}
