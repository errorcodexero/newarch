#include "CollectorTubAction.h"
#include "Collector.h"
#include "Intake.h"
#include "IntakeDutyCycleAction.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::string CollectorTubAction::action_name("CollectorTubAction");

        CollectorTubAction::CollectorTubAction(Collector &col) : CollectorAction(col){
        }
        CollectorTubAction::~CollectorTubAction() {            
        }
        
        std::string CollectorTubAction::toString(State st) {
            std::string ret ;

            switch (st) {
            case State::reset:
                ret = "reset" ;
                break ;         
            case State::waiting:
                ret = "waiting" ;
                break ;         
            case State::cancel:
                ret = "cancel" ;
                break ;
            }

            return ret ;
        }
/*
            getCollector().getIntake()->setAction(intake_dir_p) ;
            auto intake_dir_p = std::make_shared<IntakeDutyCycleAction>(*getCollector().getIntake(), "intake:speed:collect") ;

            state_ = State::waiting ;
*/
        void CollectorTubAction::run() {
            switch(state_) {
            case State::reset:
                state_ = State::waiting ;
                break ;

            case State::cancel:
                break ;
            }

            if (state_ != prev_state_) {
                MessageLogger &logger = getCollector().getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_COLLECTOR) ;
                logger << "Collected: changed states '" ;
                logger << toString(prev_state_) ;
                logger << "' -> '" ;
                logger << toString(state_) ;
                logger.endMessage() ;
            }

            prev_state_ = state_ ;
        }

        void CollectorTubAction::cancel() {
            auto action = getCollector().getIntake()->getAction() ;
            if (action != nullptr)
                action->cancel() ;

        }
    }
}