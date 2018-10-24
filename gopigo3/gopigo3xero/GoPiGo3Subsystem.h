#pragma once

#include "ServoSubsystem.h"
#include <subsystem.h>
#include <tankdrive/TankDrive.h>

namespace xero {
	namespace gopigo {
		class GoPiGo3Subsystem : public xero::base::Subsystem {
		public:
			GoPiGo3Subsystem(xero::base::Robot &robot) ;
			virtual ~GoPiGo3Subsystem() ;

			std::shared_ptr<ServoSubsystem> getServoSubsystem() {
				return servo_ ;
			}

			std::shared_ptr<xero::base::TankDrive> getTankDriveSubsystem() {
				return tankdrive_ ;
			}

		private:
			std::shared_ptr<ServoSubsystem> servo_ ;
			std::shared_ptr<xero::base::TankDrive> tankdrive_ ;
		} ;
	}
}