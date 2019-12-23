#include "CollectCubeAction.h"
#include "Collector.h"
#include "grabber/Grabber.h"
#include "phoenixids.h"
#include <motorencodersubsystem/MotorEncoderGoToAction.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::base ;
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
            case State::cancel:
                ret = "cancel" ;
                break ;
            }

            return ret ;
        }

        void CollectCubeAction::start() {
            auto grabber_dir_p = std::make_shared<MotorEncoderGoToAction>(*getCollector().getGrabber(), "grabber:angle:collect") ;
            getCollector().getGrabber()->setAction(grabber_dir_p) ;

            auto intake_dir_p = std::make_shared<SingleMotorPowerAction>(*getCollector().getIntake(), "intake:power:collect") ;
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

                    auto grabber_dir_p = std::make_shared<SingleMotorPowerAction>(*getCollector().getGrabber(), "intake:power:hold") ;
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
                    auto intake_dir_p = std::make_shared<SingleMotorPowerAction>(*getCollector().getIntake(), "intake:power:hold") ;                   
                    getCollector().getIntake()->setAction(intake_dir_p) ;
                    getCollector().setCollectedCubeState(true) ;
                    state_ = State::clamp ;
                }
                break ;

            case State::clamp:
                if (!getCollector().hasCube()) {
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

        bool CollectCubeAction::isDone() {

            return state_ == State::clamp || state_ == State::cancel ;
        }

        void CollectCubeAction::cancel() {
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
