#include "TubManipulatorEjectAction.h"
#include "TubManipulatorAction.h"
#include "TubManipulatorSubsystem.h"
#include <actions/Action.h>
#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace ranseur {

        std::string TubManipulatorEjectAction::action_name("TubManipulatorEjectAction");

        TubManipulatorEjectAction::TubManipulatorEjectAction(TubManipulatorSubsystem &tubm) : TubManipulatorAction(tubm) {
            double v ;                       //value
            ActionPtr act ;                  //action
            auto collector = tubm.getTubCollector() ;
            auto arm = tubm.getTubArm() ;
            auto wrist = tubm.getTubWrist() ;

            /// Arm ///
            v = tubm.getRobot().getSettingsParser().getDouble("tubarm:eject:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            parallel_.addSubActionPair(arm, act) ;

            /// Wrist ///
            v =tubm.getRobot().getSettingsParser().getDouble("tubwrist:eject:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            parallel_.addSubActionPair(wrist, act) ;     

            ///   ^^^ grab the angle values from the dat files ^^^   ///
            ///add the action and the subsytem to the parallel action///

        }

        TubManipulatorEjectAction::~TubManipulatorEjectAction() {            
        }
        
        void TubManipulatorEjectAction::start() {
            parallel_.start() ;
        }

        void TubManipulatorEjectAction::run() {
            parallel_.run() ;
        }

        bool TubManipulatorEjectAction::isDone() {
            return parallel_.isDone() ;
        }

        void TubManipulatorEjectAction::cancel() {
            parallel_.cancel() ;
        }
        
    }
}
