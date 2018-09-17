#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"

namespace xero {
	namespace base {
		/// \brief Drives the drivebase straight for a given distance
		class TankDriveCharAction : public TankDriveAction {
		public:
			TankDriveCharAction(TankDrive &subsystem, double duration) ;
			virtual ~TankDriveCharAction() ;

			void start();
			void run();
			void cancel();
			bool isDone();
			std::string toString();

		private:
			double start_time_ ;
			double duration_ ;
			bool is_done_ ;
		} ;
	}
}

