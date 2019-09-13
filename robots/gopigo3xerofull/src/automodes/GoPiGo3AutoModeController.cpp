#include "GoPiGo3AutoModeController.h"
#include "LedAutoMode.h"
#include <tankdrive/modes/FollowPathAutomode.h>
#include <tankdrive/modes/TankDriveScrubMode.h>
#include <tankdrive/modes/StraightCharAutoMode.h>

using namespace xero::base ;

namespace xero {
    namespace gopigo3 {
        GoPiGo3AutoModeController::GoPiGo3AutoModeController(Robot &robot) : AutoController(robot) {

        }

        GoPiGo3AutoModeController::~GoPiGo3AutoModeController() {           
        }

        void GoPiGo3AutoModeController::updateAutoMode(int sel, const std::string &gamedata) {
            AutoModePtr ptr = nullptr ;
            sel = 8 ;
            
            switch(sel)
            {
            case 0:
                ptr = std::make_shared<LedAutoMode>(getRobot()) ;
                break ;
            case 1:
                break ;
            case 2:
                break ;
            case 3:
                break ;
            case 4:
                break ;
            case 5:
                break ;
            case 6:
                break;
            case 7:
                ptr = std::make_shared<TankDriveScrubMode>(getRobot(), 0.5, 4 * 360.0) ;            
                break ;
            case 8:
                ptr = std::make_shared<StraightCharAutoMode>(getRobot(), 0.5, 5.0) ;            
                break ;
            case 9:
                ptr = std::make_shared<FollowPathAutomode>(getRobot(), "TestPaths_RightTurn") ;
                break ;
            }
            setAction(ptr) ;
        }

#ifdef NOTYET

        AutoModePtr GoPiGo3AutoModeController::createBlinkEyes()
        {
            ActionPtr action;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot());
            auto sub = xerorobot.getGoPiGoSubsystem();
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "LED");
            auto servo = sub->getLEDSubsystem();

            action = std::make_shared<LEDSubsystemBlinkAction>(*servo, LEDSubsystem::LED::LeftEye, 0.5, 0.5, 0.0, 1.0, 1.0);
            seq->pushSubActionPair(servo, action, false);

            return seq;
        }

        AutoModePtr GoPiGo3AutoModeController::createLEDAutoMode()
        {

        }

        AutoModePtr GoPiGo3AutoModeController::createServoAutoMode()
        {
            ActionPtr action;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot());
            auto sub = xerorobot.getGoPiGoSubsystem();
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "Servo");
            auto servo = sub->getServoSubsystem();

            action = std::make_shared<ServoGoToAngle>(*servo, 0);
            seq->pushSubActionPair(servo, action);

            return seq;
        }

        AutoModePtr GoPiGo3AutoModeController::createRotateNeg90() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotateNeg90") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, -90.0) ;
            seq->pushSubActionPair(db, action) ;            

            return seq ;                
        }       

        AutoModePtr GoPiGo3AutoModeController::createRotatePos90() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotatePos90") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, 90.0) ;
            seq->pushSubActionPair(db, action) ;            

            return seq ;                
        }

        AutoModePtr GoPiGo3AutoModeController::createRotatePos45() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotatePos45") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, 45.0) ;            
            seq->pushSubActionPair(db, action) ;        

            return seq ;                    
        }

        AutoModePtr GoPiGo3AutoModeController::createRotatePos135() {
            ActionPtr action ;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
            auto sub = xerorobot.getGoPiGoSubsystem() ;
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "RotatePos135") ;
            auto db = sub->getTankDrive() ;

            action = std::make_shared<TankDriveAngleAction>(*db, 135.0) ;
            seq->pushSubActionPair(db, action) ;                

            return seq ;            
        }
#endif        
    }
}
