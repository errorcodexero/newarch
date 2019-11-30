#pragma once

#include "ranseuroi/RanseurOISubsystem.h"
#include "ranseurcameratracker/RanseurCameraTracker.h"
#include "bunnyarm/BunnyArm.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include <MessageLogger.h>
#include <tubmanipulatorsubsystem/TubManipulatorSubsystem.h>
#include <frc/DigitalOutput.h>

namespace xero {
    namespace ranseur {
        class RanseurRobotSubsystem : public xero::base::RobotSubsystem {
        public:     
            RanseurRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~RanseurRobotSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<RanseurOISubsystem> getOI() {
                return oi_ ;
            }    

            std::shared_ptr<RanseurCameraTracker> getCameraTracker() {
                return tracker_ ;
            }            

            std::shared_ptr<BunnyArm> getBunnyArm() {
                return bunnyArm_;
            }

             std::shared_ptr<TubManipulatorSubsystem> getTubManipulatorSubsystem() {
                return manipulator_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

            void setDIOState(int index, bool state) {
                xero::misc::MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(xero::misc::MessageLogger::MessageType::error) ;
                logger << "Setting LED " << index << " to state " ;
                if (state)
                    logger << "on" ;
                else
                    logger << "off" ;
                logger.endMessage() ;                
                dios_[index]->Set(!state) ;
            }
            
        private:
            std::shared_ptr<RanseurOISubsystem> oi_ ;
            std::shared_ptr<RanseurCameraTracker> tracker_ ;
            std::shared_ptr<BunnyArm> bunnyArm_;
            std::shared_ptr<TubManipulatorSubsystem> manipulator_ ;
            std::vector<std::shared_ptr<frc::DigitalOutput>> dios_ ;
        } ;
    }
}
