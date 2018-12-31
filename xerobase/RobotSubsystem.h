#pragma once

#include "DriveBase.h"
#include "Subsystem.h"

/// \file

namespace xero {
	namespace base {
        /// \brief this is the base class for the whole robot subsystem
		class RobotSubsystem : public Subsystem {
		public:
            /// \brief create the whole robot subsystem
            /// \param robot the robot this subsystem belongs to
            /// \param name the name  of the subsystem
			RobotSubsystem(xero::base::Robot &robot, const std::string &name) ;

            /// \brief destroy the subsystem
			virtual ~RobotSubsystem() ;

		protected:
            /// \brief add a tank drive to the robot
            /// The hardware parameters for the tank drive are extacted from the
            /// settings file and used to popules the motors, encoders, shifter
            /// solenoids, etc.
			void addTankDrive() ;

            /// \brief return the drive base
            /// \returns a pointer to the drivebase
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