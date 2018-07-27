#include "Drivebase.h"
#include <SPI.h>

namespace xero {
	namespace base {
		Drivebase::Drivebase(Robot &robot) : Subsystem(robot, "drivebase"), navx_(frc::SPI::Port::kMXP) {
		}

		void Drivebase::computeState() {
			angle_ = navx_.GetYaw();
		}

		Drivebase::~Drivebase() {
		}
	}
}
