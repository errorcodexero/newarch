#include "Dylan.h"
#include "dylangroups.h"
#include <ActionSequence.h>
#include <DelayAction.h>
#include <TeleopController.h>
#include <basegroups.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>

#ifdef SIMULATOR
#include <DylanSimulator.h>
#endif

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace dylan {

        Dylan::Dylan() : xero::base::Robot("Dylan", 0.02) {           
        }

        void Dylan::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();
                        
            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            // logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
            // logger.enableSubsystem(MSG_GROUP_ACTIONS);
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;
            // logger.enableSubsystem(MSG_GROUP_OI) ;
            //
            
            logger.enableSubsystem(MSG_GROUP_ALL) ;         
        }
        
        void Dylan::RobotHardwareInit() {
#ifdef NOTYET
            auto sub_p = std::make_shared<DylanRobotSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getTankDrive()) ;
#endif
        }

        std::shared_ptr<ControllerBase> Dylan::createAutoController() {
#ifdef NOTYET
            auto ctrl = std::make_shared<DylanAutoController>(*this) ;
            return ctrl ;
#endif 
            return nullptr ;
        }
        
        std::shared_ptr<ControllerBase> Dylan::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> Dylan::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


