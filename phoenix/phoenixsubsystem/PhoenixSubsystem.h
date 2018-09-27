#pragma once

#include "phoenixoi/PhoenixOI.h"
#include "wings/Wings.h"
#include "liftingcollector/LiftingCollector.h"
#include <tankdrive/TankDrive.h>
#include <Subsystem.h>

namespace xero {
	namespace phoenix {
		class PhoenixSubsystem : public xero::base::Subsystem {
		public:
			PhoenixSubsystem(xero::base::Robot &robot) ;
			virtual ~PhoenixSubsystem() ;

			xero::base::SubsystemPtr getOI() {
				return oi_ ;
			}

			xero::base::SubsystemPtr getDriveBase() {
				return db_ ;
			}

		private:
			std::shared_ptr<xero::base::TankDrive> db_ ;
			std::shared_ptr<PhoenixOI> oi_ ;
			std::shared_ptr<Wings> wings_ ;
			std::shared_ptr<LiftingCollector> liftcollect_ ;
		} ;
	}
}