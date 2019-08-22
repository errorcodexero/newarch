#pragma once

#include "swerveoi/SwerveOISubsystem.h"
#include "swervedrive/SwerveDriveBase.h"
#include <RobotSubsystem.h>

namespace xero {
    namespace swerve {

        //
        // Add gamemanipulator
        // Add climber
        //
        class SwerveRobotSubsystem : public xero::base::RobotSubsystem {
        public:
            SwerveRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~SwerveRobotSubsystem() ;

            std::shared_ptr<xero::base::SwerveDriveBase> getTankDrive() {
                return db_ ;
            }
          
            std::shared_ptr<SwerveOISubsystem> getOI() {
                return oi_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

            std::shared_ptr<xero::base::SwerveDriveBase> getSwerveDrive() {
                return db_ ;
            }
            
        private:
            std::shared_ptr<SwerveOISubsystem> oi_ ;
            std::shared_ptr<xero::base::SwerveDriveBase> db_ ;
        } ;
    }
}