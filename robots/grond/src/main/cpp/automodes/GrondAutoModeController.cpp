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
            ActionSequencePtr mode = nullptr ;

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

        ActionSequencePtr GrondAutoModeController::createCharAutoMode() {
            std::string name = "Char Drive Base" ;
            std::string desc = "Characterize the drive base" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &grond = dynamic_cast<Grond &>(getRobot()) ;
            auto grondrobot = grond.getGrondRobotSubsystem() ;
            auto db = grondrobot->getTankDrive() ;

            act = std::make_shared<TankDriveCharAction>(*db, 2.0, 0.7) ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }        

        ActionSequencePtr GrondAutoModeController::createTestOne() {
            std::string name = "Test One" ;
            std::string desc = "Drive straight" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &grond = dynamic_cast<Grond &>(getRobot()) ;
            auto grondrobot = grond.getGrondRobotSubsystem() ;
            auto db = grondrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "TestOne") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }

        ActionSequencePtr GrondAutoModeController::createTestTwo() {
            std::string name = "Test Two" ;
            std::string desc = "Drive Curve" ;
            ActionSequencePtr mode = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), name, desc) ;
            ActionPtr act ;

            auto &grond = dynamic_cast<Grond &>(getRobot()) ;
            auto grondrobot = grond.getGrondRobotSubsystem() ;
            auto db = grondrobot->getTankDrive() ;

            act = std::make_shared<TankDriveFollowPathAction>(*db, "TestTwo") ;
            mode->pushSubActionPair(db, act) ;

            return mode ;         
        }        
    }
}
