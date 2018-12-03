#include "SorterStageBallAction.h"
#include "Robot.h"
#include "bunnyids.h"
#include <xeromath.h>
#include <cmath>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
		SorterStageBallAction::SorterStageBallAction(Sorter &sorter, Sorter::BallColor color) : SorterAlignCapableAction(sorter) {
			color_ = color ;
			intake_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:intake_power") ;
			output_power = sorter.getRobot().getSettingsParser().getDouble("sorter:output_power") ;
			keep_ball_motor_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:keep_power") ;
			keep_ball_duration_ = sorter.getRobot().getSettingsParser().getDouble("sorter:keep_duration") ;
			reject_ball_motor_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:reject_power") ;
			reject_ball_duration_ = sorter.getRobot().getSettingsParser().getDouble("sorter:reject_duration") ;
		}

		SorterStageBallAction::~SorterStageBallAction() {
		}

		void SorterStageBallAction::start() {
			state_ = State::AlignSorter ;
			setTargetAngle(calcTargetAngle()) ;
		}

		void SorterStageBallAction::run() {
			auto &logger = getSubsystem().getRobot().getMessageLogger() ;
			auto &sorter = getSubsystem() ;

			switch(state_) {
			case State::WaitingForBall:
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SoterStageBallAction: State WaitingForBall" ;
				logger.endMessage() ;

				if (sorter.hasBall()) {
					kick_start_ = getSubsystem().getRobot().getTime() ;
					if (sorter.getBallColor() == color_) {
						//
						// Keep ball
						//
						getSubsystem().setIntakeMotor(0.0) ;
						getSubsystem().setSorterMotor(keep_ball_motor_power_) ;
						state_ = State::KeepingBall ;
					}
					else {
						//
						// Reject ball
						//
						getSubsystem().setIntakeMotor(0.0) ;						
						getSubsystem().setSorterMotor(reject_ball_motor_power_) ;
						state_ = State::RejectingBall ;
					}
				}
				else {
					sorter.setIntakeMotor(intake_power_) ;
				}
				break ;

			case State::KeepingBall:
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SoterStageBallAction: State KeepingBall" ;
				logger.endMessage() ;

				if (getSubsystem().getRobot().getTime() - kick_start_ > keep_ball_duration_) {
					getSubsystem().setSorterMotor(0) ;
					state_ = State::AlignSorter ;
					setTargetAngle(calcTargetAngle()) ;
				}
				break ;

			case State::RejectingBall:
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SoterStageBallAction: State RejectingBall" ;
				logger.endMessage() ;

				if (getSubsystem().getRobot().getTime() - kick_start_ > reject_ball_duration_) {
					getSubsystem().setSorterMotor(0) ;
					state_ = State::AlignSorter ;
					setTargetAngle(calcTargetAngle()) ;
				}                
				break ;

			case State::AlignSorter:
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SoterStageBallAction: State AlignSofter" ;
				logger.endMessage() ;

				if (alignSorter()) {
					state_ = State::WaitingForBall ;
					getSubsystem().setIntakeMotor(intake_power_) ;
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
