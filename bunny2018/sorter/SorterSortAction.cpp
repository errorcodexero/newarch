#include "SorterSortAction.h"
#include "Robot.h"
#include <cmath>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterSortAction::SorterSortAction(Sorter &sorter, Sorter::BallColor color) : SorterAction(sorter), calc_(5,0.0) {
				auto & settings = sorter.getRobot().getSettingsParser() ;
                color_ = color ;
                state_ = State::Waiting ;
                ctrl_.initFromSettingsExtended(settings, "sorter:pid") ;
                reject_angle_ = settings.getDouble("sorter:reject_angle") ;
                keep_angle_ = settings.getDouble("sorter:keep_angle") ;
            }

            SorterSortAction::~SorterSortAction() {
            }

            std::string SorterSortAction::toString(SorterSortAction::State st) {
                std::string ret = "INVALID" ;
                switch(st) {
                    case State::NotValid:
                        ret = "NotValid" ;
                        break ;

                    case State::Waiting:
                        ret = "Waiting" ;
                        break ;

                    case State::IntakeDelay:
                        ret = "IntakeDelay" ;
                        break ;

                    case State::ColorSense:
                        ret = "ColorSensor" ;
                        break ;

                    case State::ColorSenseDelay:
                        ret = "ColorSenseDelay" ;
                        break ;

                    case State::NextHole:
                        ret = "NextHole" ;
                        break ;
                }

                return ret ;
            }

            void SorterSortAction::start() {
                state_ = State::Waiting ;
                last_state_ = State::NotValid ;
                getSubsystem().setIntakeMotor(-0.60) ;
            }

            void SorterSortAction::run() {
                if (last_state_ != state_) {
                    std::cout << "SorterSortActon: " ;
                    std::cout << toString(last_state_) << " -> " << toString(state_) ;
                    std::cout << std::endl ;
                    last_state_ = state_ ;
                }
                auto &sorter = getSubsystem() ;
                switch(state_) {
                case State::NotValid:
                    break; 
                case State::Waiting:
                    if (sorter.hasBall()) {
                        state_ = State::ColorSense ;
                        start_time_ = sorter.getRobot().getTime() ;
                    }
                    break ;

                case State::IntakeDelay:
                    if (sorter.getRobot().getTime() - start_time_ > 0.100)
                    {
                        state_ = State::ColorSense ;
                        start_time_ = sorter.getRobot().getTime() ;
                    }
                    break ;

                case State::ColorSense:
                    if (sorter.hasBall()) {
                        if (sorter.getBallColor() == color_ || color_ == Sorter::BallColor::None) {
                            dir_ = 1.0 ;
                            state_ = State::ColorSenseDelay ;
                        }
                        else {
                            dir_ = -1.0 ;
                            state_ = State::ColorSenseDelay ;
                        }
                    }
                    else if (sorter.getRobot().getTime() - start_time_ > 60) {
                        dir_ = 1.0 ;
                        state_ = State::ColorSenseDelay ;
                    }

                    if (state_ == State::ColorSenseDelay) {
                        start_time_ = sorter.getRobot().getTime() ;
                        sorter.setIntakeMotor(0.0) ;
                    }

                    break ;

                case State::ColorSenseDelay:
                    if (sorter.getRobot().getTime() - start_time_ > 0.200) {
                        state_ = State::NextHole ;
                        sorter.setSorterMotor(dir_ * 0.20) ;                        
                    }
                    break ;

                case State::NextHole:
                    if (!sorter.wheel_sensor_->Get()) {
                        sorter.setSorterMotor(0.0) ;
                        state_ = State::Waiting ;
                        getSubsystem().setIntakeMotor(-0.60) ;
                    }
                    break ;
                }
            }

            bool SorterSortAction::isDone() {
                return true ;
            }

            void SorterSortAction::cancel() {
            }

            std::string SorterSortAction::toString() {
                std::string result = "SorterSortAction " ;
				if (color_ == Sorter::BallColor::Red)
					result += "red" ;
				else if (color_ == Sorter::BallColor::Blue)
					result += "blue" ;
				else
					result = "all" ;

                return result ;
            }
    }
}
