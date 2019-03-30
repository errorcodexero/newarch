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
#include <frc/DriverStation.h>
#include <frc/DigitalInput.h>
#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace phaser {

        Phaser::Phaser() : xero::base::Robot("phaser", 0.02) {   
            comp_bot_flag_set_ = false ;
        }

        bool Phaser::isCompBot() {
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
            // logger.enableSubsystem(MSG_GROUP_VISION_TERMINATOR);             
            // logger.enableSubsystem(MSG_GROUP_PHASER_TURNTABLE);
            // logger.enableSubsystem(MSG_GROUP_PHASER_TURNTABLE_VERBOSE);                  
            // logger.enableSubsystem(MSG_GROUP_PHASER_LIFTER);
            // logger.enableSubsystem(MSG_GROUP_CARGO_INTAKE);    
            // logger.enableSubsystem(MSG_GROUP_CARGO_HOLDER);   
            // logger.enableSubsystem(MSG_GROUP_HATCH_HOLDER);
            // logger.enableSubsystem(MSG_GROUP_HATCH_HOLDER_VERBOSE);            
            // logger.enableSubsystem(MSG_GROUP_PHASER_OI) ;  
            // logger.enableSubsystem(MSG_GROUP_PARSER) ;   
            // logger.enableSubsystem(MSG_GROUP_ALL) ;                              
            // logger.enableSubsystem(MSG_GROUP_READY_ACTION) ;
            // logger.enableSubsystem(MSG_GROUP_STRAFE); 

            logger.enableSubsystem(MSG_GROUP_ACTIONS);               
            logger.enableSubsystem(MSG_GROUP_OI) ;
            logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE); 
            logger.enableSubsystem(MSG_GROUP_PHASER_OI) ;    
            logger.enableSubsystem(MSG_GROUP_HATCH_HOLDER);

            //
            // This should stay on.  It will have no effect on the real robot
            // but will show information about injected events in the simulation
            // environment.
            //
            logger.enableSubsystem(MSG_GROUP_SIMULATOR) ;
        }
        
        void Phaser::loadPaths() {
            auto paths = getPathManager() ;

            // Center two hatch automode left ls
            paths->loadPath("CenterHab2CargoFrontRight") ;              // CenterHab2CargoFrontRight.pf1.csv
            paths->loadPath("CargoFrontRightLSLeft") ;                  // CargoFrontRightLSLeft.pf1.csv
            paths->loadPath("LSLeftCargoFrontLeft") ;                   // LSLeftCargoFrontLeft.pf1.csv
            
            // Center two hatch automode right ls
            paths->loadPath("CenterHab2CargoFrontLeft") ;               // CenterHab2CargoFrontLeft.pf1.csv
            paths->loadPath("CargoFrontLeftLSRight") ;                  // CargoFrontLeftLSRight.pf1.csv
            paths->loadPath("LSRightCargoFrontRight") ;                 // LSRightCargoFrontRight.pf1.csv

            // Left rocket two hatch
            paths->loadPath("LeftHABLeftRocket") ;                      // LeftHABLeftRocket.pf1.csv
            paths->loadPath("LeftRocketLSLeft") ;                       // LeftRocketLSLeft.pf1.csv
            paths->loadPath("LSLeftRocketLeft") ;                       // LSLeftRocketLeft.pf1.csv

            // Right rocket two hatch
            paths->loadPath("RightHABRightRocket") ;                    // RightHABRightRocket.pf1.csv
            paths->loadPath("RightRocketLSRight") ;                     // RightRocketLSRight.pf1.csv
            paths->loadPath("LSRightRocketRight") ;                     // LSRightRocketRight.pf1.csv
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


