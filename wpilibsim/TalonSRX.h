#pragma once

#include "SimulatedObject.h"
#include "SensorCollection.h"

namespace ctre {
namespace phoenix {
namespace motorcontrol {
        enum class ControlMode {
            PercentOutput,
            Position,
            Velocity,
            Current,
            Follower,
            MotionProfile,
            MotionMagic,
            MotionMagicArc,
            MotionProfileArc,
            Disabled
        } ;

        enum class NeutralMode {
            Brake,
            Coast
        } ;

namespace can {
    class TalonSRX : public xero::sim::SimulatedObject
    {
        friend xero::sim::RobotSimBase ;
    public:
        TalonSRX(int index);
        virtual ~TalonSRX();

        virtual void Set(ControlMode mode, double v);
        virtual double Get() {
            return value_ ;
        }

        int GetDeviceID() {
            return index_ ;
        }

        void SetInverted(bool inverted) {
            inverted_ = inverted ;
        }

        SensorCollection& GetSensorCollection() ;

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
        SensorCollection sensors_ ;
    };
}
}
}
}
