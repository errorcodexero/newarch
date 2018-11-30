#include "SorterStageBallAction.h"
#include "Robot.h"
#include <xeromath.h>
#include <cmath>

using namespace xero::base;
using namespace xero::misc ;


namespace xero {
    namespace bunny2018 {
            SorterStageBallAction::SorterStageBallAction(Sorter &sorter, Sorter::BallColor color) : SorterAction(sorter) {
                color_ = color ;
				intake_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:intake_power") ;
				output_power = sorter.getRobot().getSettingsParser().getDouble("sorter:output_power") ;
				keep_ball_motor_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:keep_power") ;
				keep_ball_duration_ = sorter.getRobot().getSettingsParser().getDouble("sorter:keep_duration") ;
				reject_ball_motor_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:reject_power") ;
				reject_ball_duration_ = sorter.getRobot().getSettingsParser().getDouble("sorter:reject_duration") ;
				hole_tolerance_ = sorter.getRobot().getSettingsParser().getDouble("sorter:tolerance") ;
				next_hole_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:search_power") ;
				int hole_count = sorter.getRobot().getSettingsParser().getInteger("sorter:holes") ;
				double hole_size = sorter.getRobot().getSettingsParser().getDouble("sorter:hole_width") ;
				double hole_offset = sorter.getRobot().getSettingsParser().getDouble("sorter:hole_offset") ;

				for(int i = 0 ; i < hole_count ; i++)
					holes_.push_back(i * 360.0 / hole_count + hole_offset) ;
            }

            SorterStageBallAction::~SorterStageBallAction() {
            }

            void SorterStageBallAction::start() {
                state_ = State::AlignSorter ;
                getSubsystem().setIntakeMotor(intake_power_) ;
            }

			double SorterStageBallAction::calcTargetAngle() {
				double ret = holes_[0] ;
				double v = getSubsystem().getSorterMotorPower() ;
				double angle = getSubsystem().getAngle() ;

				if (v < 0.05) {
					//
					// The sorter motor is stopped, find the next opening that is the closest
					//
					double diff = std::numeric_limits<double>::max() ;
					for(double a : holes_) {
						double mydiff = std::fabs(a - angle) ;
						if (mydiff < diff) {
							ret = a ;
							diff = mydiff ;
						}
					}
				}
				else if (v < 0.0) {
					//
					// The motor is already moving in a negative direction, find the angle that is
					// next but less than the current angle
					//
					double diff = std::numeric_limits<double>::max() ;
					for(double a : holes_) {
						double mydiff = std::fabs(a - angle) ;
						if (mydiff < diff && a < angle) {
							ret = a ;
							diff = mydiff ;
						}
					}					
				}
				else if (v > 0.0) {
					//
					// The motor is already moving in a positive direction, find the angle that is
					// next but greater than the current angle
					//
					double diff = std::numeric_limits<double>::max() ;
					for(double a : holes_) {
						double mydiff = std::fabs(a - angle) ;
						if (mydiff < diff && a > angle) {
							ret = a ;
							diff = mydiff ;
						}
					}						
				}

				return ret ;
			}

            bool SorterStageBallAction::alignSorter() {
				bool ret = false ;
				double angle = getSubsystem().getAngle() ;
				double diff = xero::math::normalizeAngleDegrees(target_angle_ - angle) ;

				if (std::fabs(diff) < hole_tolerance_) {
					getSubsystem().setSorterMotor(0.0) ;
					ret = true ;
				}
				else if (diff > 0) {
					getSubsystem().setSorterMotor(next_hole_power_) ;
				}
				else {
					getSubsystem().setSorterMotor(-next_hole_power_) ;					
				}

				return ret ;
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
						target_angle_ = calcTargetAngle() ;						
                    }
                    break ;

                case State::RejectingBall:
                    if (getSubsystem().getRobot().getTime() - kick_start_ > reject_ball_duration_) {
                        getSubsystem().setSorterMotor(0) ;
                        state_ = State::AlignSorter ;
						target_angle_ = calcTargetAngle() ;
                    }                
                    break ;

                case State::AlignSorter:
                    if (alignSorter()) {
						state_ = State::Done ;
					}
                    break ;
                }
            }

            bool SorterStageBallAction::isDone() {
                return state_ == State::Done ;
            }

            void SorterStageBallAction::cancel() {
            }

            std::string SorterStageBallAction::toString() {
                std::string result = "SorterStageBallAction " ;
                return result ;
            }
    }
}
