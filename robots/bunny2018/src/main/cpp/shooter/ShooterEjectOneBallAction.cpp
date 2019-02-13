#include <assert.h>
#include <Robot.h>
#include "ShooterEjectOneBallAction.h"

namespace xero {
    namespace bunny2018 {
        
            ShooterEjectOneBallAction::ShooterEjectOneBallAction(Shooter& shooter) : ShooterStageBallAction(shooter) {
                state_= State::NOT_STARTED;
            }

            ShooterEjectOneBallAction::~ShooterEjectOneBallAction() {
            }

            void ShooterEjectOneBallAction::start() {
                last_state_ = State::NOT_STARTED ;
                if (getSubsystem().getBallIsStaged()) {
                    if (ejectTrigger()) {
                        std::cout << "Eject Trigger in start" << std::endl ;
                        readyEjecting() ;
                        state_ = State::EJECTING ;
                    }
                    else {
                        state_ = State::WAITING ;
                    }
                }
                else {
                    state_ = State::STAGING ;
                    readyStaging() ;
                }
            }

            void ShooterEjectOneBallAction::readyStaging() {
                const double motor_power = getSubsystem().getStageMotorPower();
                getSubsystem().setMotor(motor_power);                
            }

            void ShooterEjectOneBallAction::readyEjecting() {
                eject_start_time_ = getTime() ;
                getSubsystem().setMotor(getSubsystem().getEjectMotorPower()) ;
            }

            std::string ShooterEjectOneBallAction::toString(ShooterEjectOneBallAction::State st) {
                std::string ret = "???" ;
                switch(st) {
                    case State::NOT_STARTED:
                        ret = "NOT_STARTED" ;
                        break ;

                    case State::STAGING:
                        ret = "STAGING" ;
                        break ;

                    case State::WAITING:
                        ret = "WAITING" ;
                        break ;

                    case State::EJECTING:
                        ret = "EJECTING" ;
                        break ;

                    case State::DONE:
                        ret = "DONE" ;
                        break ;
                }

                return ret ;
            }

            void ShooterEjectOneBallAction::run() {
                bool b ;
                if (isDone())
                    return ;

                switch(state_) {
                    case State::NOT_STARTED:
                        assert(false) ;
                        break ;

                    case State::STAGING:
                        if (ShooterStageBallAction::isDone()) {
                            if (ejectTrigger()) {
                                readyEjecting() ;
                                state_ = State::EJECTING ;
                            }
                            else {
                                getSubsystem().setMotor(0.0) ;
                                state_ = State::WAITING ;
                            }
                        }
                        break ;

                    case State::WAITING:
                        b = ejectTrigger() ;
                        if (b) {
                            std::cout << "ejectTrigger in run" << std::endl ;
                            readyEjecting() ;
                            state_ = State::EJECTING ;
                        }
                        break ;

                    case State::EJECTING:
                        if (getTime() - eject_start_time_ > getSubsystem().getEjectDuration()) {
                            getSubsystem().setMotor(0.0) ;
                            getSubsystem().clearBallIsStaged() ;

                            if (repeatEjectOne()) {
                                readyStaging() ;
                                state_ = State::STAGING ;
                            } else {
                                state_ = State::DONE ;
                            }
                        }
                        break ;

                    case State::DONE:
                        break ;
                }

                if (last_state_ != state_) {
                    std::cout << "State: " << toString(last_state_) << " -> " << toString(state_) << std::endl ;
                    last_state_ = state_ ;
                }
            }

            bool ShooterEjectOneBallAction::isDone() {
                return state_ == State::DONE;                
            }

            void ShooterEjectOneBallAction::cancel() {
                getSubsystem().setMotor(0);
                state_ = State::DONE ;
            }

            std::string ShooterEjectOneBallAction::toString() {
                std::string result = "ShooterEjectOneBallAction";

                result += " staged=";
                if (getSubsystem().getBallIsStaged())
                    result += "true";
                else
                    result += "false";

                result += " state=";
                switch (state_) {
                    case State::WAITING:
                        result += "WAITING" ;
                        break ;
                    case State::NOT_STARTED:
                        result += "NOT_STARTED";
                        break;
                    case State::STAGING:
                        result += "STAGING";
                        break;
                    case State::EJECTING:
                        result += "EJECTING";
                        break;
                    case State::DONE:
                        result += "DONE";
                        break;
                }

                return result ;
            }

            double ShooterEjectOneBallAction::getTime() {
                return getSubsystem().getRobot().getTime();
            }
    }
}
