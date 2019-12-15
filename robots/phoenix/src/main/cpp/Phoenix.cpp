#include "Phoenix.h"
#include "phoenixgroups.h"
#include "automodes/PhoenixAutoController.h"
#include <actions/SequenceAction.h>
#include <actions/DelayAction.h>
#include <TeleopController.h>
#include <basegroups.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>

#ifdef SIMULATOR
#include <PhoenixSimulator.h>
#endif

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace phoenix {

        Phoenix::Phoenix() : xero::base::Robot("phoenix", 0.02) {           
        }

        void Phoenix::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();
                        
            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            // logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
            // logger.enableSubsystem(MSG_GROUP_ACTIONS);
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;
            // logger.enableSubsystem(MSG_GROUP_OI) ;
            //            
            logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
            logger.enableSubsystem(MSG_GROUP_ACTIONS);
        }

        void Phoenix::loadPaths() {
            auto paths = getPathManager() ;
            // paths->loadPath("TestPathOne") ;
        }
        
        void Phoenix::RobotHardwareInit() {
            auto sub_p = std::make_shared<PhoenixRobotSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getTankDrive()) ;
        }

        std::shared_ptr<ControllerBase> Phoenix::createAutoController() {
            auto ctrl = std::make_shared<PhoenixAutoController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> Phoenix::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> Phoenix::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


