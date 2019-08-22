#include "SwerveRobot.h"
#include "swerverobotsubsystem.h"
#include "swerveids.h"
#include <ActionSequence.h>
#include <DelayAction.h>
#include <TeleopController.h>
#include <basegroups.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>
#include <frc/DriverStation.h>
#include <frc/DigitalInput.h>
#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace swerve {

        Swerve::Swerve() : xero::base::Robot("phaser", 0.02) {   
            comp_bot_flag_set_ = false ;
        }

        bool Swerve::isCompBot() {
            return true ;
        }

        void Swerve::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();
                        
            //
            // Decide what message groups (incl. subsystems) you want to see
            //       
            // logger.enableSubsystem(MSG_GROUP_ACTIONS);
            // logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE);               
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;

            //
            // This should stay on.  It will have no effect on the real robot
            // but will show information about injected events in the simulation
            // environment.
            //
            logger.enableSubsystem(MSG_GROUP_SIMULATOR) ;
        }
        
        void Swerve::loadPaths() {
        }
        
        void Swerve::RobotHardwareInit() {
            auto sub_p = std::make_shared<SwerveRobotSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getSwerveDriveBase()) ;
        }

        std::shared_ptr<ControllerBase> Swerve::createAutoController() {
            auto ctrl = std::make_shared<SwerveAutoModeController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> Swerve::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> Swerve::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


