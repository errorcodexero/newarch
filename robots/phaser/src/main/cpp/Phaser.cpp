#include "Phaser.h"
#include "phaserids.h"
#include "automodes/PhaserAutoModeController.h"
#include <ActionSequence.h>
#include <DelayAction.h>
#include <TeleopController.h>
#include <basegroups.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>
#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace phaser {

        Phaser::Phaser() : xero::base::Robot("phaser", 0.02) {           
        }

        void Phaser::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();
                        
            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            // logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
            // logger.enableSubsystem(MSG_GROUP_ACTIONS);
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;
            // logger.enableSubsystem(MSG_GROUP_OI) ;
            // logger.enableSubsystem(MSG_GROUP_ALL) ;         
            //

            logger.enableSubsystem(MSG_GROUP_TANKDRIVE);            
            // logger.enableSubsystem(MSG_GROUP_TANKDRIVE_VERBOSE);            
        }

        void Phaser::loadPaths() {
            auto paths = getPathManager() ;
            paths->loadPath("TestOne");            
            paths->loadPath("TestTwo") ;     
        }
        
        void Phaser::RobotHardwareInit() {
            auto sub_p = std::make_shared<PhaserRobotSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getTankDrive()) ;
        }

        std::shared_ptr<ControllerBase> Phaser::createAutoController() {
            auto ctrl = std::make_shared<PhaserAutoModeController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> Phaser::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> Phaser::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


