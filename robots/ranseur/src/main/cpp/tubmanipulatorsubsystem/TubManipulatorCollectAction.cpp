#include "TubManipulatorCollectAction.h"
#include "TubManipulatorAction.h"
#include "TubManipulatorSubsystem.h"
#include "tubcollector/TubCollectorTubAction.h"
#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace ranseur {

        std::string TubManipulatorCollectAction::action_name("TubManipulatorCollectAction");

        TubManipulatorCollectAction::TubManipulatorCollectAction(TubManipulatorSubsystem &tubm) : TubManipulatorAction(tubm) {
            double v ;                                          //value
            ActionPtr act ;                                     //action
            auto collector = tubm.getTubCollector() ;
            auto arm = tubm.getTubArm() ;
            auto wrist = tubm.getTubWrist() ;

            ///Arm/// 
            v = tubm.getRobot().getSettingsParser().getDouble("tubarm:collect:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            parallel_.addSubActionPair(arm, act) ;

            ///Wrist///
            v = tubm.getRobot().getSettingsParser().getDouble("tubwrist:collect:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            parallel_.addSubActionPair(wrist, act) ;

            /// Collector ///
            act = std::make_shared<TubCollectorTubAction>(*collector) ;
            parallel_.addSubActionPair(collector, act) ;

            ///   ^^^ grab the angle values from the dat files ^^^   ///
            ///add the action and the subsytem to the parallel action///

        }

        TubManipulatorCollectAction::~TubManipulatorCollectAction() {            
        }
        
        void TubManipulatorCollectAction::start() {
            parallel_.start() ;
        }

        void TubManipulatorCollectAction::run() {
            parallel_.run() ;
        }

        bool TubManipulatorCollectAction::isDone() {
            return parallel_.isDone(); 
        }

        void TubManipulatorCollectAction::cancel() {
            parallel_.cancel() ;
        }

    }
}
