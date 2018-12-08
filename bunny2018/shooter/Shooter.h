#pragma once

#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <DigitalInput.h>

namespace xero {
    namespace bunny2018 {
		
        class Shooter : public xero::base::SingleMotorSubsystem {

            /// The action to set the motor power is derived from SingleMotorSubsystem

            /// \brief Run the belt at reduced speed until a ball is detected by the sensor, then stop the belt (motor).
            friend class ShooterStageBallAction;

            /// \brief If a ball is staged, run belt at high speed for short period of time to eject it then stop.
            // If no ball is staged, run the belt at stage speed until the ball is detected, and then switch to higher speed temporarily to eject it. Then stop.
            friend class ShooterEjectOneBallAction;
            
        public:
            
            Shooter(xero::base::Robot& robot);
            virtual ~Shooter(); 
            virtual void computeState();
            virtual bool canAcceptAction(xero::base::ActionPtr action);

        private:

            double getStageMotorPower() const {
                return stage_motor_power_;
            }

            double getStageEjectMotorPower() const {
                return stage_motor_power_;
            }

            double getBallIsStaged() const {
                return ball_is_staged_;
            }


        private:
            std::shared_ptr<frc::DigitalInput> ball_sensor_;

            double stage_motor_power_;
            double eject_motor_power_;
            bool   ball_is_staged_;
        };
    }
}
