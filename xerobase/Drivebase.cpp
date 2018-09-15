#include "Drivebase.h"
#include <SPI.h>

namespace xero {
	namespace base {
		Drivebase::Drivebase(Robot &robot) : Subsystem(robot, "drivebase") {
#ifdef GOPIGO
			navx_ = new AHRS("/dev/ttyACM0") ;	
#else
			navx_ = new AHRS(frc::SPI::Port::kMXP) ;		
#endif
		}

		Drivebase::~Drivebase() {
			if (navx_ != nullptr)
				delete navx_ ;
		}

		void Drivebase::computeState() {
			angle_ = navx_->GetYaw();
		}
	}
}
