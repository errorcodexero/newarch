
#include "GoPiGo3Xero.h"
#include "gopigo3groups.h"
#include "GoPiGo3AutoModeController.h"
#include "GoPiGo3Subsystem.h"
#include <ActionSequence.h>
#include <basegroups.h>
#include <DelayAction.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>
#include <TeleopController.h>
#include <TestController.h>

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace gopigo {

        GoPiGo3Xero::GoPiGo3Xero() : xero::base::Robot("gopigo3", 0.02) {
        }

        std::shared_ptr<GoPiGo3Subsystem> GoPiGo3Xero::getGoPiGoSubsystem() {
            auto sub = getRobotSubsystem() ;
            return std::dynamic_pointer_cast<GoPiGo3Subsystem>(sub) ;
        }

        void GoPiGo3Xero::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();

            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            // logger.enableSubsystem(MSG_GROUP_ALL) ;          
            logger.enableSubsystem(MSG_GROUP_TANKDRIVE);            
        }

        void GoPiGo3Xero::LoadPaths() {
            auto pathmgr = getPathManager() ;
            pathmgr->loadPath("TestPathOne") ;
        }

        void GoPiGo3Xero::RobotHardwareInit() {
            auto robot_p = std::make_shared<GoPiGo3Subsystem>(*this) ;
            auto oi = robot_p->getOI() ;
            auto db = robot_p->getTankDrive() ;
            assert(oi != nullptr) ;
            assert(db != nullptr) ;

            setRobotSubsystem(robot_p, oi, db) ;
        }

        std::shared_ptr<ControllerBase> GoPiGo3Xero::createAutoController() {
            return std::make_shared<GoPiGo3AutoModeController>(*this) ;
        }
        
        std::shared_ptr<ControllerBase> GoPiGo3Xero::createTeleopController() {
            return std::make_shared<TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> GoPiGo3Xero::createTestController() {
            return std::make_shared<TestController>(*this) ;
        }
    }
}

//
// This macro call is defined by the WPI library.  It is the call that connects
// the class xero::GoPiGo3Xero::Phoenix to physical robot
//
START_ROBOT_CLASS(xero::gopigo::GoPiGo3Xero) ;
