#include "CollectorTubAction.h"
#include "Collector.h"
#include "CollectorDutyCycleAction.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::string CollectorTubAction::action_name("CollectorTubAction");

        CollectorTubAction::CollectorTubAction(Collector &col) : CollectorAction(col){
            speed_ = col.getRobot().getSettingsParser().getDouble("tubcollect:collectspeed") ;
            delay_ = col.getRobot().getSettingsParser().getDouble("tubcollect:collectdelay") ;
        }
        CollectorTubAction::~CollectorTubAction() {            
        }
        

        void CollectorTubAction::start() {
            getCollector().openHand() ;
            state_ = State::motors_on ;
            getCollector().getIntake()->set(speed_) ;
        }

        void CollectorTubAction::run() {
            if(state_ == State::motors_on) {
                if(getCollector().hasTub()) {
                    getCollector().closeHand() ;
                    state_ = State::closed_hand ;
                    start_ = getCollector().getRobot().getTime() ;
                }
            }
            else if(state_ == State::closed_hand) {
                if(getCollector().getRobot().getTime() - start_ > delay_) {
                    getCollector().getIntake()->set(0.0) ;
                    state_ = State::done ;
                }
            }
        }

        bool CollectorTubAction::isDone() {
            return state_ == State::done ;
        }

        void CollectorTubAction::cancel() {
            getCollector().getIntake()->set(0.0) ;
            getCollector().closeHand() ;
            state_ = State::done ;
        }
    }
}
