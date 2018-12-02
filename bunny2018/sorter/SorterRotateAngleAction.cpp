#include "SorterRotateAngleAction.h"
#include "Robot.h"
#include <xeromath.h>

using namespace xero::base;
using namespace xero::misc ;

namespace xero {
    namespace bunny2018 {
            SorterRotateAngleAction::SorterRotateAngleAction(Sorter &sorter, double angle, double power) : SorterAction(sorter) {
				target_angle_ = angle ;
				if(target_angle_ < 0)
					target_angle_ = 0 ;
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
			}

            void SorterRotateAngleAction::start() {
				total_angle_ = 0 ;
				is_done_ = false ;
				calcNextAngle() ;

				getSubsystem().setSorterMotor(power_) ;
            }

            void SorterRotateAngleAction::run() {
				if (!is_done_) {
					if (getSubsystem().getAngle() > next_angle_) {
						total_angle_ += next_angle_size_ ;
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
