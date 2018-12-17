#include <assert.h>
#include <Robot.h>
#include "ShooterEjectOneBallAction.h"

namespace xero {
    namespace bunny2018 {
        
            ShooterEjectOneBallAction::ShooterEjectOneBallAction(Shooter& shooter) : ShooterAction(shooter) {
                state_= State::NOT_STARTED;
            }

            ShooterEjectOneBallAction::~ShooterEjectOneBallAction() {
            }

            void ShooterEjectOneBallAction::start() {
                state_= State::STAGING;
                if (!getSubsystem().getBallIsStaged()) {
                    const double motor_power = getSubsystem().getStageMotorPower();
                    getSubsystem().setMotor(motor_power);
                }
            }

            void ShooterEjectOneBallAction::run() {

                if (isDone()) {
                    // Do nothing.  Ball already ejected.
                } else if (getSubsystem().getBallIsStaged() && state_ != State::EJECTING) {
                    // Ball detected by sensor. Switch from staging to ejecting and maintain eject speed for
                    // specified period duration.
                    // Note that once a ball is staged, it can only be cleared by an eject action even
                    // if the ball is no longer seen by the sensor.
                    assert(state_ == State::STAGING);
                    state_ = State::EJECTING;
                    eject_start_time_ = getTime();
                    const double motor_power = getSubsystem().getEjectMotorPower();
                    getSubsystem().setMotor(motor_power);
                } else if (state_ == State::EJECTING && ((getTime() - eject_start_time_) >= getSubsystem().getEjectDuration())) {
                    // In the process of ejecting and done with ejecting, stop motor and complete action
                    state_ = State::DONE;
                    getSubsystem().setMotor(0);
					getSubsystem().clearBallIsStaged() ;
                } else if (state_ == State::EJECTING) {
                    // In the process of ejecting and NOT done with ejecting.  Do nothing.
                } else {
                    // Continue staging
                    assert(state_ == State::STAGING);
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
