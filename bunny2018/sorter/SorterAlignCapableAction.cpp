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
				next_hole_power_ = sorter.getRobot().getSettingsParser().getDouble("sorter:search_power") ;
				int hole_count = sorter.getRobot().getSettingsParser().getInteger("sorter:holes") ;
				double hole_size = sorter.getRobot().getSettingsParser().getDouble("sorter:hole_width") ;
				double hole_offset = sorter.getRobot().getSettingsParser().getDouble("sorter:hole_offset") ;

				for(int i = 0 ; i < hole_count ; i++)
					holes_.push_back(i * 360.0 / hole_count + hole_offset) ;				
            }

            SorterAlignCapableAction::~SorterAlignCapableAction() {
            }

		double SorterAlignCapableAction::calcTargetAngle() {
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

		bool SorterAlignCapableAction::alignSorter() {
			auto &logger = getSubsystem().getRobot().getMessageLogger() ;				
			bool ret = false ;
			double angle = getSubsystem().getAngle() ;
			double diff = xero::math::normalizeAngleDegrees(target_angle_ - angle) ;

			logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
			logger << "SoterStageBallAction: alignSorter" ;
			logger << ", target angle " << target_angle_ ;
			logger << ", current angle " << angle ;
			logger.endMessage() ;				

			if (std::fabs(diff) < hole_tolerance_) {
				logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SORTER) ;
				logger << "SoterStageBallAction: alignSorter complete" ;
				logger.endMessage() ;	

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
    }
}
