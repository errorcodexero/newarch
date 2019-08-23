#include "SwerveRobot.h"
#include "swerverobotsubsystem/SwerveRobotSubsystem.h"
#include "automodes/SwerveAutoModeController.h"
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

        SwerveRobot::SwerveRobot() : xero::base::Robot("swerve", 0.02) {   
        }

        SwerveRobot::~SwerveRobot()
        {
        }

        bool SwerveRobot::isCompBot() {
            return true ;
        }

        void SwerveRobot::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();
                        
            //
            // Decide what message groups (incl. subsystems) you want to see
            //       
            logger.enableSubsystem(MSG_GROUP_ACTIONS);
            logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE);               
            logger.enableSubsystem(MSG_GROUP_PARSER) ;

            //
            // This should stay on.  It will have no effect on the real robot
            // but will show information about injected events in the simulation
            // environment.
            //
            logger.enableSubsystem(MSG_GROUP_SIMULATOR) ;
        }
        
        void SwerveRobot::loadPaths() {
        }
        
        void SwerveRobot::RobotHardwareInit() {
            auto sub_p = std::make_shared<SwerveRobotSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getSwerveDrive()) ;

            std::cout << "In RobotHardwareInit" << std::endl ;
        }

        std::shared_ptr<ControllerBase> SwerveRobot::createAutoController() {
            auto ctrl = std::make_shared<SwerveAutoModeController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> SwerveRobot::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> SwerveRobot::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


