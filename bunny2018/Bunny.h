#pragma once

#include <Robot.h>
#include <tankdrive/TankDrive.h>

namespace xero {
	namespace bunny2018 {

		class BunnySubsystem ;

		/// \brief the concrete class that represents the robot Phoenix
		class Bunny : public xero::base::Robot {
		public:
			Bunny() ;

			/// \brief return the bunny robot subsystem
			std::shared_ptr<BunnySubsystem> getBunnySubsystem() ;

			virtual void DoDisabledWork() ;

		protected:
			/// \brief called to initialize the robot, basically creating the subsystems
			virtual void RobotInit() ;

			/// \brief called to create the autonomous controller
			/// \returns a robot controller
			virtual std::shared_ptr<xero::base::ControllerBase> createAutoController() ;
			
			/// \brief called to create the teleop controller
			/// \returns a robot controller
			virtual std::shared_ptr<xero::base::ControllerBase> createTeleopController() ;

			/// \brief called to create the test controller
			/// \returns a robot controller
			virtual std::shared_ptr<xero::base::ControllerBase> createTestController() ;



		private:
			void initializeMessageLogger();
		} ;
	}
}
