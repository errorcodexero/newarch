#include "Droid.h"
#include "droidids.h"
#include "automodes/DroidAutoModeController.h"
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
    namespace droid {

        std::array<uint8_t, 6> Droid::practice_bot_mac_addr_ = { 0x00, 0x80, 0x2F, 0x17, 0x89, 0x91 } ;

        Droid::Droid() : xero::base::Robot("droid", 0.02) 
        {
        }

        bool Droid::isCompBot() {
            auto mac = getAddr() ;

            if (mac.size() != practice_bot_mac_addr_.size())
                return true ;

            if (memcmp(&mac[0], &practice_bot_mac_addr_[0], mac.size()) == 0)
                return false ;

            return true ;
        }

        void Droid::enableSpecificMessages() {
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
            // logger.enableSubsystem(MSG_GROUP_SHOOTER);
            // logger.enableSubsystem(MSG_GROUP_TURRET); 
            // logger.enableSubsystem(MSG_GROUP_CONVEYOR);      
            // logger.enableSubsystem(MSG_GROUP_INTAKE);                

            logger.enableSubsystem(MSG_GROUP_ACTIONS);  
            logger.enableSubsystem(MSG_GROUP_ACTIONS_VERBOSE);    
            logger.enableSubsystem(MSG_GROUP_OI) ;

            //
            // This should stay on.  It will have no effect on the real robot
            // but will show information about injected events in the simulation
            // environment.
            //
            logger.enableSubsystem(MSG_GROUP_SIMULATOR) ;
        }
        
        void Droid::loadPaths() {
            auto paths = getPathManager() ;


            paths->setExtensions("_left.csv", "_right.csv") ;
            paths->loadPath("eight_ball_auto_collect") ;
            paths->loadPath("eight_ball_auto_collect2") ;
            paths->loadPath("eight_ball_auto_fire") ;    
            paths->loadPath("eight_ball_auto_fire2") ;   
            paths->loadPath("five_ball_auto_collect") ;  
            paths->loadPath("five_ball_auto_fire") ;  
            paths->loadPath("three_ball_auto_backup") ;    
            paths->loadPath("three_ball_auto_fire") ;                                                                     
        }
        
        void Droid::RobotHardwareInit() {
            auto sub_p = std::make_shared<DroidSubsystem>(*this) ;
            setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getTankDrive()) ;
        }

        std::shared_ptr<ControllerBase> Droid::createAutoController() {
            auto ctrl = std::make_shared<DroidAutoModeController>(*this) ;
            return ctrl ;
        }
        
        std::shared_ptr<ControllerBase> Droid::createTeleopController() {
            //
            // The base teleop controller is sufficient
            //
            return std::make_shared<xero::base::TeleopController>(*this) ;
        }
         
        std::shared_ptr<ControllerBase> Droid::createTestController() {
            //
            // This is where the test controller is created
            //
            return nullptr ;
        }
    }
}


