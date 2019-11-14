#include "TubManipulatorDumpAction.h"
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

        std::string TubManipulatorDumpAction::action_name("TubManipulatorDumpAction") ;

        TubManipulatorDumpAction::TubManipulatorDumpAction(TubManipulatorSubsystem &tubm) : TubManipulatorAction(tubm) {
            double v ;                                          //value to store angles of arm and wrist
            ActionPtr act ;                                     //action
            auto collector = tubm.getTubCollector() ;
            auto arm = tubm.getTubArm() ;
            auto wrist = tubm.getTubWrist() ;

            ///Arm/// 
            v = tubm.getRobot().getSettingsParser().getDouble("tubarm:dump:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            parallel_.addSubActionPair(arm, act) ;

            ///Wrist///
            v = tubm.getRobot().getSettingsParser().getDouble("tubwrist:dump:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            parallel_.addSubActionPair(wrist, act) ;
            
            /// ^^^ grab the angle values from the dat files ^^^
            ///add the action and the subsytem to the parallel action
        }

        TubManipulatorDumpAction::~TubManipulatorDumpAction() {            
        }
        
        void TubManipulatorDumpAction::start() {
            parallel_.start() ;
        }

        void TubManipulatorDumpAction::run() {
            parallel_.run() ;
        }

        bool TubManipulatorDumpAction::isDone() {
            return parallel_.isDone() ;
        }

        void TubManipulatorDumpAction::cancel() {
            parallel_.isDone() ;
        }

    }
}
