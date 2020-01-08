#pragma once

#include <frc/SimulatedObject.h>

namespace rev
{
class CANSparkMaxLowLevel;

    class CANEncoder : public xero::sim::SimulatedObject
    {
    public:
        CANEncoder(CANSparkMaxLowLevel &motor);
        virtual ~CANEncoder();

        double GetPosition();
        void SetPosition(double pos) ;
        void SimulatorSetPosition(double pos);

        void SetPositionConversionFactor(double mult) {
            pos_mult_ = mult;
        }

        void SetVelocityConversionFactor(double mult) {
            vel_mult_ = mult;
        }

        void SetInverted(bool inv) {
            reverse_ = inv;
        }

        bool GetInverted() {
            return reverse_;
        }

        CANSparkMaxLowLevel *SimulatorGetMotor() {
            return motor_;
        }

    private:
        CANSparkMaxLowLevel *motor_;
        double pos_mult_;
        double vel_mult_;
        double pos_;
        bool reverse_;
    };
}