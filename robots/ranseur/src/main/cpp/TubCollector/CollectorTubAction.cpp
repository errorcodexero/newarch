#include "CollectTubAction.h"
#include "Collector.h"
#include "intake/Intake.h"
#include "grabber/Grabber.h"
#include "intake/IntakeDutyCycleAction.h"
#include "grabber/GrabberToAngleAction.h"
#include "grabber/GrabberHoldTubAction.h"
#include "phoenixgroups.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace phoenix {

        std::string CollectTubAction::action_name("CollectTub") ;

        CollectTubAction::CollectTubAction(Collector &col) : CollectorAction(col) {
            grab_time_ = col.getRobot().getSettingsParser().getDouble("collector:grab_time") ;
            prev_state_ = State::reset ;
        }

        CollectTubAction::~CollectTubAction() {            
        }

        std::string CollectTubAction::toString(State st) {
            std::string ret ;

            switch (st) {
            case State::reset:
                ret = "reset" ;
                break ;         
            case State::waiting:
                ret = "waiting" ;
                break ;         
            case State::clamp:
                ret = "clamp" ;
                break ;         
            case State::grabbing:
                ret = "grabbing" ;
                break ;
            case State::cancel:
                ret = "cancel" ;
                break ;
            }

            return ret ;
        }

        void CollectTubAction::start() {
            auto grabber_dir_p = std::make_shared<GrabberToAngleAction>(*getCollector().getGrabber(), "grabber:angle:collect") ;
            getCollector().getGrabber()->setAction(grabber_dir_p) ;

            auto intake_dir_p = std::make_shared<IntakeDutyCycleAction>(*getCollector().getIntake(), "intake:speed:collect") ;
            getCollector().getIntake()->setAction(intake_dir_p) ;

            state_ = State::waiting ;
        }

        void CollectTubAction::run() {
            switch(state_) {
            case State::reset:
                state_ = State::waiting ;
                break ;

            case State::waiting:
                if (getCollector().hasTub()) {
                    state_ = State::grabbing ;
                    start_ = getCollector().getRobot().getTime() ;

                    auto grabber_dir_p = std::make_shared<GrabberHoldTubAction>(*getCollector().getGrabber()) ;
                    getCollector().getGrabber()->setAction(grabber_dir_p) ;
                }
                break ;

            case State::grabbing:
                if (!getCollector().hasTub()) {
                    //
                    // We lost the Tub, start collecting again
                    //
                    start() ;
                }
                else if (getCollector().getRobot().getTime() - start_ > grab_time_) {
                    //
                    // We really have the Tub now, go to the hold state
                    //
                    auto intake_dir_p = std::make_shared<IntakeDutyCycleAction>(*getCollector().getIntake(), 0) ;                   
                    getCollector().getIntake()->setAction(intake_dir_p) ;
                    getCollector().setCollectedTubState(true) ;
                    state_ = State::clamp ;
                }

            case State::clamp:
                if (!getCollector().hasTub()) {
                    start() ;
                }
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

        bool CollectTubAction::isDone() {

            return state_ == State::clamp || state_ == State::cancel ;
        }

        void CollectTubAction::cancel() {
            auto action = getCollector().getIntake()->getAction() ;
            if (action != nullptr)
                action->cancel() ;

            action = getCollector().getGrabber()->getAction() ;
            if (action != nullptr)
                action->cancel() ;
            state_ = State::cancel ;
        }
    }
}
