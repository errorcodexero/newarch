#include "MotorController.h"
#include "motorcontrollerids.h"
#include "automodes/MotorControllerAutoModeController.h"
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

////$$ StartRobotDefines
////$$ EndRobotDefines

namespace xero {
    namespace motorcontroller {

        MotorController::MotorController() : xero::base::Robot("motorcontroller", 0.02) {
            comp_bot_flag_set_ = false ;
        }

        bool MotorController::isCompBot() {
            //
            // For the practice bot, we have a jumper between digital IO 6 and ground.  For the
            // competition bot we have nothing and the roborio pulls this value up to one.  In this
            // way we detect the difference between the COMP bot and the PRACTICE bot.
            //
            if (!comp_bot_flag_set_) {
                frc::DigitalInput input(6) ;
                comp_bot_ = input.Get() ;
            }
            return comp_bot_ ;
        }

        void MotorController::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();
                        
            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            // logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
            // logger.enableSubsystem(MSG_GROUP_TANKDRIVE_VERBOSE);            
            // logger.enableSubsystem(MSG_GROUP_ACTIONS);
            // logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE);               
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;
            // logger.enableSubsystem(MSG_GROUP_OI) ;
            // logger.enableSubsystem(MSG_GROUP_ALL) ;         
            // logger.enableSubsystem(MSG_GROUP_PATHWATCHER); 

            logger.enableSubsystem(MSG_GROUP_ACTIONS);         
            logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE);                     
            logger.enableSubsystem(MSG_GROUP_OI) ;
            logger.enableSubsystem(MSG_GROUP_PATHWATCHER); 
            logger.enableSubsystem(MSG_GROUP_TESTSUBSYSTEM); 
            //logger.enableSubsystem(MSG_GROUP_COLORSENSOR);             

            //
            // This should stay on.  It will have no effect on the real robot
            // but will show information about injected events in the simulation
            // environment.
            //
            logger.enableSubsystem(MSG_GROUP_SIMULATOR) ;
        }
        
        void MotorController::loadPaths() {
            auto paths = getPathManager() ;


            paths->setExtensions("_left.csv", "_right.csv") ;

            // paths->loadPath("PATH_NAME_HERE") ;
        }
        
        void MotorController::RobotHardwareInit() {
            auto sub_p = std::make_shared<MotorControllerSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getTankDrive()) ;
        }

        std::shared_ptr<ControllerBase> MotorController::createAutoController() {
            auto ctrl = std::make_shared<MotorControllerAutoModeController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> MotorController::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> MotorController::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


