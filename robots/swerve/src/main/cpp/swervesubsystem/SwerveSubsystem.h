#pragma once

#include "swerveoi/SwerveOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace swerve {

        //
        // Add gamemanipulator
        // Add climber
        //
        class SwerveSubsystem : public xero::base::RobotSubsystem {
        public:
            SwerveSubsystem(xero::base::Robot &robot) ;
            virtual ~SwerveSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<SwerveOISubsystem> getOI() {
                return oi_ ;
            }    

            std::shared_ptr<xero::base::SingleMotorSubsystem> getMotor1() {
                return motor1_ ;
            }   

            std::shared_ptr<xero::base::SingleMotorSubsystem> getMotor2() {
                return motor2_ ;
            }  

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<SwerveOISubsystem> oi_ ;
            std::shared_ptr<xero::base::SingleMotorSubsystem> motor2_ ;
            std::shared_ptr<xero::base::SingleMotorSubsystem> motor1_ ;            
        } ;
    }
}
