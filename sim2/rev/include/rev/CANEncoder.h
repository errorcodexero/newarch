#pragma once

namespace rev
{
class CANSparkMaxLowLevel;

    class CANEncoder 
    {
    public:
        CANEncoder(CANSparkMaxLowLevel &motor);
        virtual ~CANEncoder();

        double GetPosition();
        void SetPosition(double pos) ;

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
        bool reverse_;
    };
}