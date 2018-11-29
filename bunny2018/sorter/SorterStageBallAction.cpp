#include "SorterStageBallAction.h"
#include "Robot.h"

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterStageBallAction::SorterStageBallAction(Sorter &sorter, Sorter::BallColor color) : SorterAction(sorter) {
                intake_power_ = 0.4 ;
                color_ = color ;
            }

            SorterStageBallAction::~SorterStageBallAction() {
            }

            void SorterStageBallAction::start() {
                state_ = State::WaitingForBall ;
                getSubsystem().setIntakeMotor(intake_power_) ;
            }

            void SorterStageBallAction::alignSorter() {
            }

            void SorterStageBallAction::run() {
                auto &sorter = getSubsystem() ;

                switch(state_) {
                case State::WaitingForBall:
                    if (sorter.hasBall()) {
                        kick_start_ = getSubsystem().getRobot().getTime() ;
                        if (sorter.getBallColor() == color_) {
                            //
                            // Keep ball
                            //
                            getSubsystem().setSorterMotor(keep_ball_motor_power_) ;
                            state_ = State::KeepingBall ;
                        }
                        else {
                            //
                            // Reject ball
                            //
                            getSubsystem().setSorterMotor(reject_ball_motor_power_) ;
                            state_ = State::RejectingBall ;
                        }
                    }
                    break ;

                case State::KeepingBall:
                    if (getSubsystem().getRobot().getTime() - kick_start_ > keep_ball_duration_) {
                        getSubsystem().setSorterMotor(0) ;
                        state_ = State::AlignSorter ;
                    }
                    break ;

                case State::RejectingBall:
                    if (getSubsystem().getRobot().getTime() - kick_start_ > reject_ball_duration_) {
                        getSubsystem().setSorterMotor(0) ;
                        state_ = State::AlignSorter ;
                    }                
                    break ;

                case State::AlignSorter:
                    alignSorter() ;
                    break ;
                }
            }

            bool SorterStageBallAction::isDone() {
                return true ;
            }

            void SorterStageBallAction::cancel() {
            }

            std::string SorterStageBallAction::toString() {
                std::string result = "SorterStageBallAction " ;
                return result ;
            }
    }
}
