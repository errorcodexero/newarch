#include "TubManipulatorEjectAction.h"
#include "TubManipulatorAction.h"
#include "TubManipulatorSubsystem.h"
#include "tubcollector/TubCollectorEjectTubAction.h"
#include <actions/Action.h>
#include <MessageLogger.h>
#include <Robot.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
    namespace ranseur {

        std::string TubManipulatorEjectAction::action_name("TubManipulatorEjectAction");

        TubManipulatorEjectAction::TubManipulatorEjectAction(TubManipulatorSubsystem &tubm) : TubManipulatorAction(tubm), seq_(tubm.getRobot().getMessageLogger()) {
            double v ;                       //value
            ActionPtr act ;                  //action
            auto collector = tubm.getTubCollector() ;
            auto arm = tubm.getTubArm() ;
            auto wrist = tubm.getTubWrist() ;
 
            //put the parallel action(of arm and wrist) and the collector(eject action) in a sequence

            auto par = std::make_shared<ParallelAction>(tubm.getRobot().getMessageLogger()) ; //parallel action
            seq_.pushAction(par) ;                          // put above par_ into the sequence

            /// Arm ///
            v = tubm.getRobot().getSettingsParser().getDouble("tubarm:eject:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            par->addSubActionPair(arm, act, true) ;                   /// add arm into parallel action

            /// Wrist ///
            v =tubm.getRobot().getSettingsParser().getDouble("tubwrist:eject:pos") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            par->addSubActionPair(wrist, act, true) ;                 /// add wrist into parallel action

            /// Collector ///
            act = std::make_shared<TubCollectorEjectTubAction>(*collector, "tubcollector:eject:speed", "tubcollector:eject:delay") ;
            seq_.pushSubActionPair(collector, act, true) ;              /// add collector into sequence action

            par = std::make_shared<ParallelAction>(tubm.getRobot().getMessageLogger()) ; //parallel action
            seq_.pushAction(par) ;                          // put above par_ into the sequence

            /// Arm ///
            v = tubm.getRobot().getSettingsParser().getDouble("tubarm:eject:finish") ;
            act = std::make_shared<MotorEncoderGoToAction>(*arm, v) ;
            par->addSubActionPair(arm, act) ;                   /// add arm into parallel action

            /// Wrist ///
            v =tubm.getRobot().getSettingsParser().getDouble("tubwrist:eject:finish") ;
            act = std::make_shared<MotorEncoderGoToAction>(*wrist, v) ;
            par->addSubActionPair(wrist, act) ;                 /// add wrist into parallel action
        }

        TubManipulatorEjectAction::~TubManipulatorEjectAction() {            
        }
        
        void TubManipulatorEjectAction::start() {
            TubManipulatorAction::start();
            seq_.start() ;
        }

        void TubManipulatorEjectAction::run() {
            TubManipulatorAction::run();
            seq_.run() ;
            if (seq_.isDone())
                setDone();
        }

        void TubManipulatorEjectAction::cancel() {
            TubManipulatorAction::cancel();
            seq_.cancel() ;
            setDone();
        }
        
    }
}
