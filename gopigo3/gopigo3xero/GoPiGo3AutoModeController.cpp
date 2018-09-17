#include "GoPiGo3AutoModeController.h"
#include "ServoGoToAngle.h"
#include "GoPiGo3Subsystem.h"
#include "GoPiGo3Xero.h"
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveCharAction.h>
#include <ActionSequence.h>
#include <DelayAction.h>
#include <Robot.h>

using namespace xero::base ;

namespace xero {
	namespace gopigo {
		GoPiGo3AutoModeController::GoPiGo3AutoModeController(Robot &robot) : AutoController(robot) {
			auto ptr = createAutoMode() ;
			setAction(ptr) ;
		}

		GoPiGo3AutoModeController::~GoPiGo3AutoModeController() {			
		}

		ActionPtr GoPiGo3AutoModeController::createAutoMode() {
			ActionPtr action ;
			GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getRobot()) ;
			auto sub = xerorobot.getRobotSubsystem() ;
			auto seq = std::make_shared<ActionSequence>(getRobot().getMessageLogger()) ;

			auto servo = sub->getServoSubsystem() ;
			auto tankdrive = sub->getTankDriveSubsystem() ;
			
#ifdef ROTATE_HEAD
			action = std::make_shared<ServoGoToAngle>(*servo, 0.0) ;
			seq->pushSubActionPair(servo, action) ;

			action = std::make_shared<DelayAction>(3.0) ;
			seq->pushAction(action) ;

			action = std::make_shared<ServoGoToAngle>(*servo, 180.0) ;
			seq->pushSubActionPair(servo, action) ;

			action = std::make_shared<DelayAction>(3.0) ;
			seq->pushAction(action) ;		

			action = std::make_shared<ServoGoToAngle>(*servo, 90.0) ;
			seq->pushSubActionPair(servo, action) ;
#endif

			action = std::make_shared<TankDriveCharAction>(*tankdrive, 5.0) ;
			seq->pushSubActionPair(tankdrive, action) ;

			return seq ;
		}
	}
}