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

        bool Phaser::isCompBot() {
            comp_bot_ = new frc::DigitalInput(6) ;
            return comp_bot_->Get() ;
        }

        void Phaser::enableSpecificMessages() {
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
            // logger.enableSubsystem(MSG_GROUP_CAMERA_TRACKER); 
            // logger.enableSubsystem(MSG_GROUP_CAMERA_TRACKER_VERBOSE);             
            // logger.enableSubsystem(MSG_GROUP_LINE_FOLLOWER);  
            // logger.enableSubsystem(MSG_GROUP_VISION_DRIVING); 
            // logger.enableSubsystem(MSG_GROUP_PHASER_TURNTABLE);
            // logger.enableSubsystem(MSG_GROUP_PHASER_LIFTER);
            // logger.enableSubsystem(MSG_GROUP_CARGO_INTAKE);    
            // logger.enableSubsystem(MSG_GROUP_CARGO_HOLDER);   
            // logger.enableSubsystem(MSG_GROUP_HATCH_INTAKE);    
            // logger.enableSubsystem(MSG_GROUP_HATCH_HOLDER);
            // logger.enableSubsystem(MSG_GROUP_PHASER_OI) ;       
            
            logger.enableSubsystem(MSG_GROUP_ACTIONS);
            logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE);    
            logger.enableSubsystem(MSG_GROUP_PHASER_OI) ;    

            //
            // This should stay on.  It will have no effect on the real robot
            // but will show information about injected events in the simulation
            // environment.
            //
            logger.enableSubsystem(MSG_GROUP_SIMULATOR) ;
        }
        
        void Phaser::loadPaths() {
            auto paths = getPathManager() ;
            paths->loadPath("Straight") ;
        }
        
        void Phaser::RobotHardwareInit() {
            compressor_ = std::make_shared<frc::Compressor>() ;
            compressor_->SetClosedLoopControl(true) ;
            compressor_->Start() ;

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


