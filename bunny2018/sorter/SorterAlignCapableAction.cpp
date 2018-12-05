#include "SorterAlignCapableAction.h"
#include "bunnyids.h"
#include <Robot.h>
#include <xeromath.h>
#include <cmath>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
		SorterAlignCapableAction::SorterAlignCapableAction(Sorter &sorter) : SorterAction(sorter) {
			hole_tolerance_ = sorter.getRobot().getSettingsParser().getDouble("sorter:tolerance") ;
			int hole_count = sorter.getRobot().getSettingsParser().getInteger("sorter:holes") ;
			double hole_offset = sorter.getRobot().getSettingsParser().getDouble("sorter:hole_offset") ;

			pid_.initFromSettingsExtended(sorter.getRobot().getSettingsParser(), "sorter:pid") ;
			calc_ = std::make_shared<SorterHoleCalculator>(hole_count, hole_offset) ;

			auto &logger = getSubsystem().getRobot().getMessageLogger() ;	
			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
			logger << "SorterAlignCapableAction: hole angles " ;
			logger << calc_->toString() ;
			logger.endMessage() ;				
		}

		SorterAlignCapableAction::~SorterAlignCapableAction() {
		}

		double SorterAlignCapableAction::calcTargetAngle() {
			double power = getSubsystem().getSorterMotorPower() ;
			double angle = getSubsystem().getAngle() ;
			return calc_->calcTargetAngle(power, angle) ;
		}

		void SorterAlignCapableAction::setTargetAngle(double a) {
			target_angle_ = a ;

			auto &logger = getSubsystem().getRobot().getMessageLogger() ;	
			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
			logger << "SorterAlignCapableAction: setTargetAngle" ;
			logger << ", target angle " << target_angle_ ;
			logger.endMessage() ;				
		}

		bool SorterAlignCapableAction::isAtTarget() {
			bool ret = false ;
			double angle = getSubsystem().getAngle() ;			
			double diff = xero::math::normalizeAngleDegrees(target_angle_ - angle) ;
			return std::fabs(diff) < hole_tolerance_ ;
		}

		bool SorterAlignCapableAction::alignSorter() {
			auto &logger = getSubsystem().getRobot().getMessageLogger() ;				
			bool ret = false ;
			double angle = getSubsystem().getAngle() ;

			if (isAtTarget()) {
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SoterStageBallAction: alignSorter complete" ;
				logger.endMessage() ;	

				getSubsystem().setSorterMotor(0.0) ;
				ret = true ;
			}
			else {
				double out = pid_.getOutput(target_angle_, angle, 0.0, getSubsystem().getRobot().getDeltaTime()) ;
				getSubsystem().setSorterMotor(out) ;

				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SorterAlignCapableAction: alignSorter" ;
				logger << ", ticks " << getSubsystem().getTicks() ;
				logger << ", target angle " << target_angle_ ;
				logger << ", current angle " << angle ;
				logger << ", output " << out ;
				logger.endMessage() ;
			}

			return ret ;
		}			
    }
}
