#include "TankDriveAngleCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

namespace xero {
	namespace base {
		TankDriveAngleCharAction::TankDriveAngleCharAction(TankDrive &drive, double duration, double value) : TankDriveAction(drive) {
			duration_ = duration ;
            value_ =value ;

		}

		TankDriveAngleCharAction::TankDriveAngleCharAction(TankDrive &drive, const std::string &name, const std::string &value_name) : TankDriveAction(drive) {
			duration_ = getTankDrive().getRobot().getSettingsParser().getDouble(name) ;
            value_ = getTankDrive().getRobot().getSettingsParser().getDouble(value_name) ;
		}		
		
		TankDriveAngleCharAction::~TankDriveAngleCharAction() {			
		}

		void TankDriveAngleCharAction::start() {
			is_done_ = false ;
			start_time_ = frc::Timer::GetFPGATimestamp() ;
			getTankDrive().setMotorsToPercents(value_, -value_) ;
			std::cout << "Char " << duration_ << std::endl ;
		}

		void TankDriveAngleCharAction::run() {
			if (!is_done_) {
				double now = frc::Timer::GetFPGATimestamp() ;
				if (now - start_time_ >= duration_) {
					std::cout << "Done: " << now << " " << start_time_ << " " << duration_ << std::endl ; 
					is_done_ = true ;
					getTankDrive().setMotorsToPercents(0.0, 0.0) ;
				} else {
					std::cout << (now - start_time_) ;
					std::cout << ", Angle " << getTankDrive().getAngle() ;
					std::cout << ", Velocity " << getTankDrive().getAngularVelocity() ;
					std::cout << ", Acceleration " << getTankDrive().getAngularAcceleration() ;
					std::cout << std::endl ;
				}
			}
		}

		void TankDriveAngleCharAction::cancel() {
			is_done_ = true ;
		}
		bool TankDriveAngleCharAction::isDone() {
			return is_done_ ;
		}

		std::string TankDriveAngleCharAction::toString() {
			std::string ret("TankDriveAngleCharAction") ;
			return ret ;
		}
	}
}