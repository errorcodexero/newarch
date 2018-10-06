#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveAngleCharAction : public TankDriveAction {
		public:
			TankDriveAngleCharAction(TankDrive &subsystem, double duration, double value) ;
			TankDriveAngleCharAction(TankDrive &subsystem, const std::string &name, const std::string &value_name) ;			
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
            double value_;
		} ;
	}
}