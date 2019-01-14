#include "GoPiGo3AutoModeController.h"
#include "ServoGoToAngle.h"
#include "GoPiGo3Subsystem.h"
#include "GoPiGo3Xero.h"
#include "ServoSubsystem.h"
#include "ServoGoToAngle.h"
#include "LEDSubsystem.h"
#include "LEDSubsystemOnOffAction.h"
#include "LEDSubsystemBlinkAction.h"
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
                ptr = createLEDAutoMode();
                break ;
            case 1:
                ptr = createServoAutoMode();
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
            case 6:
                ptr = createBlinkEyes();
                break;
            }
            setAction(ptr) ;
        }

        ActionSequencePtr GoPiGo3AutoModeController::createBlinkEyes()
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

        ActionSequencePtr GoPiGo3AutoModeController::createLEDAutoMode()
        {
            ActionPtr action;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot());
            auto sub = xerorobot.getGoPiGoSubsystem();
            auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger(), "LED");
            auto servo = sub->getLEDSubsystem();

            action = std::make_shared<LEDSubsystemOnOffAction>(*servo, LEDSubsystem::LED::LeftEye, 0.0, 1.0, 0.0);
            seq->pushSubActionPair(servo, action);

            action = std::make_shared<LEDSubsystemOnOffAction>(*servo, LEDSubsystem::LED::RightEye, 0.0, 0.0, 0.0);
            seq->pushSubActionPair(servo, action);

            action = std::make_shared<LEDSubsystemOnOffAction>(*servo, LEDSubsystem::LED::LeftBlinker, 0.0, 0.0, 0.0);
            seq->pushSubActionPair(servo, action);

            action = std::make_shared<LEDSubsystemOnOffAction>(*servo, LEDSubsystem::LED::RightBlinker, 0.0, 0.0, 0.0);
            seq->pushSubActionPair(servo, action);

            return seq;
        }

        ActionSequencePtr GoPiGo3AutoModeController::createServoAutoMode()
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
