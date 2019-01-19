
#include "Grond.h"
#include "grondsubsystem/GrondSubsystem.h"
#include "grondids.h"
#include "automodes/GrondAutoModeController.h"
#include <ActionSequence.h>
#include <basegroups.h>
#include <DelayAction.h>
#include <MessageLogger.h>
#include <frc/SmartDashboard/SmartDashboard.h>

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace grond {

        Grond::Grond() : xero::base::Robot("grond", 0.02) {
        }

        //
        // Chnage the calls to the message logger in this method to indicate what
        // messages are to be displayed. 
        //
        void Grond::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();

            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
            //logger.enableSubsystem(MSG_GROUP_ACTIONS);
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;
            // logger.enableSubsystem(MSG_GROUP_OI) ;
            // logger.enableSubsystem(MSG_GROUP_SORTER) ;
            //
        }

        void Grond::loadPaths() {
            auto paths = getPathManager() ;
            paths->loadPath("Straight");            
            paths->loadPath("Curve");     
        }        

        void Grond::RobotHardwareInit() {

            //
            // This is where the subsystems for the robot get created
            //
            auto robot_p = std::make_shared<GrondSubsystem>(*this) ;
            auto oi = robot_p->getOI() ;
            auto db = robot_p->getTankDrive() ;
            assert(oi != nullptr) ;
            assert(db != nullptr) ;
            setRobotSubsystem(robot_p, oi, db) ;     
        }

        std::shared_ptr<ControllerBase> Grond::createAutoController() {
            auto ctrl = std::make_shared<GrondAutoModeController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> Grond::createTeleopController() {
            return nullptr ;
        }
         
        std::shared_ptr<ControllerBase> Grond::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }

    }
}
