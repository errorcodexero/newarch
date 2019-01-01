#include "GoPiGo3AutoModeController.h"
#include "ServoGoToAngle.h"
#include "GoPiGo3Subsystem.h"
#include "GoPiGo3Xero.h"
#include <tankdrive/TankDriveAngleCharAction.h>
#include <tankdrive/TankDriveAngleAction.h>
#include <ActionSequence.h>
#include <DelayAction.h>
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace gopigo {
        GoPiGo3AutoModeController::GoPiGo3AutoModeController(Robot &robot) : AutoController(robot) {

        }

        GoPiGo3AutoModeController::~GoPiGo3AutoModeController() {           
        }

        void GoPiGo3AutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            ActionSequencePtr ptr = nullptr ;
            switch(sel)
            {
            case 0:
                break ;
            case 1:
                break ;
            case 2:
                ptr = createRotatePos90() ;
                break ;
            case 3:
                ptr = createRotatePos45() ;
                break ;
            case 4:
                ptr = createRotatePos135() ;
                break ;
            case 5:
                ptr = createRotateNeg90() ;
                break ;
            }
            setAction(ptr) ;
        }

        ActionSequencePtr GoPiGo3AutoModeController::createRotateNeg90() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotateNeg90") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, -90.0) ;
            seq->pushSubActionPair(db, action) ;            

            return seq ;                
        }       

        ActionSequencePtr GoPiGo3AutoModeController::createRotatePos90() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotatePos90") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, 90.0) ;
            seq->pushSubActionPair(db, action) ;            

            return seq ;                
        }

        ActionSequencePtr GoPiGo3AutoModeController::createRotatePos45() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotatePos45") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, 45.0) ;            
            seq->pushSubActionPair(db, action) ;        

            return seq ;                    
        }

        ActionSequencePtr GoPiGo3AutoModeController::createRotatePos135() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotatePos135") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, 135.0) ;
            seq->pushSubActionPair(db, action) ;                

            return seq ;            
        }
    }
}
