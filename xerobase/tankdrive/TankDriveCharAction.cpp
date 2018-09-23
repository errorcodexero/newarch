#include "TankDriveCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

namespace xero {
	namespace base {
		TankDriveCharAction::TankDriveCharAction(TankDrive &drive, double duration) : TankDriveAction(drive) {
			duration_ = duration ;
		}

		TankDriveCharAction::TankDriveCharAction(TankDrive &drive, const std::string &name) : TankDriveAction(drive) {
			duration_ = getTankDrive().getRobot().getSettingsParser().getDouble(name) ;
		}		
		
		TankDriveCharAction::~TankDriveCharAction() {			
		}

		void TankDriveCharAction::start() {
			is_done_ = false ;
			start_time_ = frc::Timer::GetFPGATimestamp() ;
			getTankDrive().setMotorsToPercents(1.0, 1.0) ;
		}

		void TankDriveCharAction::run() {
			if (!is_done_) {
				double now = frc::Timer::GetFPGATimestamp() ;
				if (now - start_time_ >= duration_) {
					is_done_ = true ;
					getTankDrive().setMotorsToPercents(0.0, 0.0) ;
				} else {
					std::cout << (now - start_time_) ;
					std::cout << ", " << getTankDrive().getDist() ;
					std::cout << ", " << getTankDrive().getVelocity() ;
					std::cout << std::endl ;
				}
			}
		}

		void TankDriveCharAction::cancel() {
			is_done_ = true ;
		}
		bool TankDriveCharAction::isDone() {
			return is_done_ ;
		}

		std::string TankDriveCharAction::toString() {
			std::string ret("TankDriveCharAction") ;
			return ret ;
		}
	}
}