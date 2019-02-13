#include "GrondAutoModeController.h"
#include "Grond.h"
#include <tankdrive/TankDriveFollowPathAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <frc/DriverStation.h>

using namespace xero::base ;

namespace xero {
    namespace grond {
        GrondAutoModeController::GrondAutoModeController(Robot &robot) : AutoController(robot) {
        }
        
        GrondAutoModeController::~GrondAutoModeController() {
        }

        void GrondAutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr mode = nullptr ;

            switch(sel) {
            case 0:
                mode = createCharAutoMode() ;
                break ;

            case 1:
                mode = createTestOne() ;
                break ;

            case 2:
                mode = createTestTwo() ;
                break ;
            }
            setAction(mode) ;
        }

        AutoModePtr GrondAutoModeController::createCharAutoMode() {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &grond = dynamic_cast<Grond &>(getRobot()) ;
            auto grondrobot = grond.getGrondRobotSubsystem() ;
            auto db = grondrobot->getTankDrive() ;

            act = std::make_shared<TankDriveCharAction>(*db, 2.0, 0.7) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }        

        AutoModePtr GrondAutoModeController::createTestOne() {
            std::string name = "Test One" ;
            std::string desc = "Path following drive straight" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &grond = dynamic_cast<Grond &>(getRobot()) ;
            auto grondrobot = grond.getGrondRobotSubsystem() ;
            auto db = grondrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "Straight") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }

        AutoModePtr GrondAutoModeController::createTestTwo() {
            std::string name = "Test Two" ;
            std::string desc = "Drive Curve" ;
            AutoModePtr mode = std::make_shared<AutoMode>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &grond = dynamic_cast<Grond &>(getRobot()) ;
            auto grondrobot = grond.getGrondRobotSubsystem() ;
            auto db = grondrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "Curve") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }        
    }
}
