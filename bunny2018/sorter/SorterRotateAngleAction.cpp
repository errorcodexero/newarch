#include "SorterRotateAngleAction.h"
#include "bunnyids.h"
#include <Robot.h>
#include <xeromath.h>
#include <cmath>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
		SorterRotateAngleAction::SorterRotateAngleAction(Sorter &sorter, double angle, double power) : SorterAction(sorter) {
			target_angle_ = angle ;
			if(target_angle_ < 0)
				target_angle_ = 0 ;

			power_ = power ;
		}

		SorterRotateAngleAction::~SorterRotateAngleAction() {
		}

		void SorterRotateAngleAction::calcNextAngle() {
			double remaining = target_angle_ - total_angle_ ;
			if (remaining > 90.0)
				next_angle_size_ = 90 ;
			else
				next_angle_size_ = remaining ;
				
			next_angle_ = xero::math::normalizeAngleDegrees(getSubsystem().getAngle() + next_angle_size_) ;

			auto &logger = getSubsystem().getRobot().getMessageLogger() ;
			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
			logger << "SorterRotateAngleAction: calcNextAngle" ;
			logger << ", target_angle_ " << target_angle_ ;
			logger << ", total_angle_ " << total_angle_ ;
			logger << ", remaining " << remaining << "\n" ;
			logger << "                                        next_angle_size_ " << next_angle_size_ ;
			logger << ", next_angle_ " << next_angle_ ;
			logger << ", current " << getSubsystem().getAngle() ;
			logger.endMessage() ;			
		}

		void SorterRotateAngleAction::start() {
			auto &logger = getSubsystem().getRobot().getMessageLogger() ;
			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
			logger << "SorterRotateAngleAction: start, target angle " << target_angle_ ;
			logger << ", power " << power_ ;
			logger.endMessage() ;

			total_angle_ = 0 ;
			is_done_ = false ;
			calcNextAngle() ;

			getSubsystem().setSorterMotor(power_) ;
		}

		void SorterRotateAngleAction::run() {
			auto &logger = getSubsystem().getRobot().getMessageLogger() ;
			double angle = getSubsystem().getAngle() ;

			if (!is_done_) {
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SorterRotateAngleAction: run, current angle " << getSubsystem().getAngle() ;
				logger << ", next_angle_ " << next_angle_ ;
				logger.endMessage() ;	

				double diff = xero::math::normalizeAngleDegrees(angle - next_angle_) ;
				if (std::fabs(diff) < 5.0) {
					total_angle_ += next_angle_size_ + angle - next_angle_ ;
					logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
					logger << "                              total_angle_ " << total_angle_ ;
					logger << ", target_angle_ " << target_angle_ ;
					logger.endMessage() ;
					if (total_angle_ > target_angle_) {
						is_done_ = true ;
						getSubsystem().setSorterMotor(0.0) ;
					}
					else {
						calcNextAngle() ;
					}
				}
			
			}
		}

		bool SorterRotateAngleAction::isDone() {
			return true ;
		}

		void SorterRotateAngleAction::cancel() {
		}

		std::string SorterRotateAngleAction::toString() {
			std::string result = "SorterRotateAngleAction " ;
			return result ;
		}
    }
}
