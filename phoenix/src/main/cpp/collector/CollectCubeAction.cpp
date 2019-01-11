#include "CollectCubeAction.h"
#include "Collector.h"
#include "intake/Intake.h"
#include "grabber/Grabber.h"
#include "intake/IntakeDutyCycleAction.h"
#include "grabber/GrabberToAngleAction.h"
#include "grabber/GrabberHoldCubeAction.h"
#include "phoenixgroups.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace phoenix {

        std::string CollectCubeAction::action_name("CollectCube") ;

        CollectCubeAction::CollectCubeAction(Collector &col) : CollectorAction(col) {
            grab_time_ = col.getRobot().getSettingsParser().getDouble("collector:grab_time") ;
            prev_state_ = State::reset ;
        }

        CollectCubeAction::~CollectCubeAction() {            
        }

        std::string CollectCubeAction::toString(State st) {
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
            }

            return ret ;
        }

        void CollectCubeAction::start() {
            auto grabber_dir_p = std::make_shared<GrabberToAngleAction>(*getCollector().getGrabber(), "grabber:angle:collect") ;
            getCollector().getGrabber()->setAction(grabber_dir_p) ;

            auto intake_dir_p = std::make_shared<IntakeDutyCycleAction>(*getCollector().getIntake(), "intake:speed:collect") ;
            getCollector().getIntake()->setAction(intake_dir_p) ;

            state_ = State::waiting ;
        }

        void CollectCubeAction::run() {
            switch(state_) {
            case State::reset:
                state_ = State::waiting ;
                break ;

            case State::waiting:
                if (getCollector().hasCube()) {
                    state_ = State::grabbing ;
                    start_ = getCollector().getRobot().getTime() ;

                    auto grabber_dir_p = std::make_shared<GrabberHoldCubeAction>(*getCollector().getGrabber()) ;
                    getCollector().getGrabber()->setAction(grabber_dir_p) ;
                }
                break ;

            case State::grabbing:
                if (!getCollector().hasCube()) {
                    //
                    // We lost the cube, start collecting again
                    //
                    start() ;
                }
                else if (getCollector().getRobot().getTime() - start_ > grab_time_) {
                    //
                    // We really have the cube now, go to the hold state
                    //
                    auto intake_dir_p = std::make_shared<IntakeDutyCycleAction>(*getCollector().getIntake(), 0) ;                   
                    getCollector().getIntake()->setAction(intake_dir_p) ;
                    getCollector().setCollectedCubeState(true) ;
                    state_ = State::clamp ;
                }

            case State::clamp:
                if (!getCollector().hasCube()) {
                    start() ;
                }
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

        bool CollectCubeAction::isDone() {
            return state_ == State::clamp ;
        }

        void CollectCubeAction::cancel() {
            auto action = getCollector().getIntake()->getAction() ;
            if (action != nullptr)
                action->cancel() ;

            action = getCollector().getGrabber()->getAction() ;
            if (action != nullptr)
                action->cancel() ;
        }
    }
}
