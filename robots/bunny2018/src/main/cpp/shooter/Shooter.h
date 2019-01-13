#pragma once

#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <frc/SmartDashboard/SmartDashboard.h>
#include <frc/DigitalInput.h>

namespace xero {
    namespace bunny2018 {
        
        class Shooter : public xero::base::SingleMotorSubsystem {

            /// The action to set the motor power is derived from SingleMotorSubsystem

            friend class ShooterStageBallAction;
            friend class ShooterEjectOneBallAction;
            friend class ShooterEjectAutoBallAction ;
            
        public:
            
            Shooter(xero::base::Robot& robot);
            virtual ~Shooter(); 
            virtual void computeState();
            virtual bool canAcceptAction(xero::base::ActionPtr action);

            void clearBallIsStaged() {
                ball_is_staged_ = false ;
                frc::SmartDashboard::PutBoolean("Staged", false) ;
            }            

        private:

            double getStageMotorPower() const {
                return stage_motor_power_;
            }

            double getEjectMotorPower() const {
                return eject_motor_power_;
            }

            double getEjectDuration() const {
                return eject_duration_;
            }

            double getBallIsStaged() const {
                return ball_is_staged_;
            }



        private:
            std::shared_ptr<frc::DigitalInput> ball_sensor_;

            // The following variables cache values read from the settings file for quick access
            double stage_motor_power_;
            double eject_motor_power_;
            double eject_duration_;

            bool   ball_is_staged_;
        };
    }
}
