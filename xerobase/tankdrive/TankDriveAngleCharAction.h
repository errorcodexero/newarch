#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveAngleCharAction : public TankDriveAction {
		public:
			TankDriveAngleCharAction(TankDrive &subsystem, double duration, double start, double stop, double incr) ;
			virtual ~TankDriveAngleCharAction() ;

			void start();
			void run();
			void cancel();
			bool isDone();
			std::string toString();

		private:
			double start_time_ ;
			double duration_ ;
			bool is_done_ ;
            double start_ ;
			double stop_ ;
			double incr_ ;
			double current_ ;
		} ;
	}
}