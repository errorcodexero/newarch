#pragma once

#include <SubsystemModel.h>

namespace xero  {
    namespace sim {
        namespace phoenix {
            class LifterModel : public SubsystemModel {
            public:
                LifterModel(RobotSimBase &simbase) ;
                virtual ~LifterModel() ;

                virtual void run(double dt);
	            virtual void inputChanged(SimulatedObject *obj);
                virtual std::string toString() ;

	            virtual void addVictorSP(frc::VictorSP *motor);
	            virtual void addEncoder(frc::Encoder *encoder);
                virtual void addDigitalInput(frc::DigitalInput *input) ;
                virtual void addSolenoid(frc::Solenoid *solenoid) ;

                double getHeight() {
                    return height_ ;
                }

                double getMotor() {
                    return voltage_ ;
                }

                bool getBrake() {
                    return brake_value_ ;
                }

                bool getHighGear() {
                    return highgear_ ;
                }

                bool getTopLimit() {
                    return height_ >= top_limit_height_ ;
                }

                bool getBottomLimit() {
                    return height_ <= bottom_limit_height_ ;
                }

            private:
                frc::Encoder *enc_ ;
                frc::DigitalInput *bottom_limit_ ;
                frc::DigitalInput *top_limit_ ;
                frc::VictorSP *motor1_ ;
                frc::VictorSP *motor2_ ;
                frc::Solenoid *brake_ ;
                frc::Solenoid *gear_ ;

                int encoder_1_ ;
                int encoder_2_ ;
                int bottom_limit_channel_ ;
                int top_limit_channel_ ;
                int motor_1_ ;
                int motor_2_ ;
                int brake_sol_ ;
                int gear_sol_ ;

                double bottom_limit_height_ ;
                double top_limit_height_ ;

                double inch_per_sec_per_volt_ ;
                double height_ ;
                double in_per_tick_ ;
                double voltage_ ;
                bool brake_value_ ;
                bool highgear_ ;
                int encoder_base_ ;
            } ;
        }
    }
}