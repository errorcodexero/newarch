#pragma once

#include "DriveBase.h"
#include "Subsystem.h"

namespace xero {
	namespace base {
		class RobotSubsystem : public Subsystem {
		public:
			RobotSubsystem(xero::base::Robot &robot, const std::string &name) ;
			virtual ~RobotSubsystem() ;

		protected:
			void addTankDrive() ;

			std::shared_ptr<DriveBase> getDriveBase() {
				return db_ ;
			}

		private:
			bool isDefinedAndTrue(const std::string &name) ;

		private:
			std::shared_ptr<DriveBase> db_ ;
		} ;
	}
}