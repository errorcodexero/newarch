#include "TubCollectorTubAction.h"
#include "TubCollector.h"
#include "TubCollectorDutyCycleAction.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::string TubCollectorTubAction::action_name("TubCollectorTubAction");

        TubCollectorTubAction::TubCollectorTubAction(TubCollector &col) : TubCollectorAction(col){
            speed_ = col.getRobot().getSettingsParser().getDouble("tubcollect:collectspeed") ;
            delay_ = col.getRobot().getSettingsParser().getDouble("tubcollect:collectdelay") ;
        }
        TubCollectorTubAction::~TubCollectorTubAction() {            
        }
        

        void TubCollectorTubAction::start() {
            getTubCollector().openHand() ;
            state_ = State::motors_on ;
            getTubCollector().getIntake()->set(speed_) ;
        }

        void TubCollectorTubAction::run() {
            if(state_ == State::motors_on) {
                if(getTubCollector().hasTub()) {
                    getTubCollector().closeHand() ;
                    state_ = State::closed_hand ;
                    start_ = getTubCollector().getRobot().getTime() ;
                }
            }
            else if(state_ == State::closed_hand) {
                if(getTubCollector().getRobot().getTime() - start_ > delay_) {
                    getTubCollector().getIntake()->set(0.0) ;
                    state_ = State::done ;
                }
            }
        }

        bool TubCollectorTubAction::isDone() {
            return state_ == State::done ;
        }

        void TubCollectorTubAction::cancel() {
            getTubCollector().getIntake()->set(0.0) ;
            getTubCollector().closeHand() ;
            state_ = State::done ;
        }
    }
}