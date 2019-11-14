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
           
            double v ;
            ActionPtr act ;
            auto collector = tubm.getTubCollector() ;
            auto arm = tubm.getTubArm() ;
            auto wrist = tubm.getTubWrist() ;

            v = tubm.getRobot().getSettingsParser().getDouble("tubarm:eject:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            parallel_.addSubActionPair(arm, act) ;


            v =tubm.getRobot().getSettingsParser().getDouble("tubwrist:eject:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            parallel_.addSubActionPair(wrist, act) ;            

        }

        TubManipulatorEjectAction::~TubManipulatorEjectAction() {            
        }
        
        void TubManipulatorEjectAction::start() {
        }

        void TubManipulatorEjectAction::run() {
        }

        bool TubManipulatorEjectAction::isDone() {
            return true;  // TEMPORARY
        }

        void TubManipulatorEjectAction::cancel() {
        }

    }
}
