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
                if (getSubsystem().getBallIsStaged()) {
                    if (ejectTrigger()) {
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

            void ShooterEjectOneBallAction::run() {
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
                                state_ = State::WAITING ;
                            }
                        }
                        break ;

                    case State::WAITING:
                        if (ejectTrigger()) {
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
            }

            bool ShooterEjectOneBallAction::isDone() {
                return state_ == State::DONE;                
            }

            void ShooterEjectOneBallAction::cancel() {
                getSubsystem().setMotor(0);
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
