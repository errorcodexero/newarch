#include "TankDriveCharAction.h"
#include "TankDrive.h"
#include <Robot.h>
#include <iostream>

namespace xero {
	namespace base {
		TankDriveCharAction::TankDriveCharAction(TankDrive &drive, double duration, double voltage, bool highgear) : TankDriveAction(drive) {
			duration_ = duration ;
			voltage_ = voltage ;
			high_gear_ = highgear ;
		}

		TankDriveCharAction::TankDriveCharAction(TankDrive &drive, const std::string &name, const std::string &voltage, bool highgear) : TankDriveAction(drive) {
			duration_ = getTankDrive().getRobot().getSettingsParser().getDouble(name) ;
			voltage_ = getTankDrive().getRobot().getSettingsParser().getDouble(voltage) ;
			high_gear_ = highgear ;
		}		
		
		TankDriveCharAction::~TankDriveCharAction() {			
		}

		void TankDriveCharAction::start() {
			is_done_ = false ;
			start_time_ = frc::Timer::GetFPGATimestamp() ;
			if (high_gear_)
				getTankDrive().highGear() ;
			else
				getTankDrive().lowGear() ;
				
			getTankDrive().setMotorsToPercents(voltage_, voltage_) ;
			std::cout << "Time,Distance,Velocity,Acceleration,TicksL,TicksR" << std::endl ;
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
					std::cout << ", " << getTankDrive().getAcceleration() ;
					std::cout << ", " << getTankDrive().getTickCountL() ;
					std::cout << ", " << getTankDrive().getTickCountR() ;
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