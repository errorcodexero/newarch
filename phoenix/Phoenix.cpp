#include <memory>
#include "Phoenix.h"

#include "MessageGroups.h"
#include "MessageLogger.h"
#include "MessageDestSeqFile.h"
#include "MessageDestStream.h"
#include "PhoenixAutoController.h"
#include "intake/IntakeDutyCycleAction.h"

#ifdef SIM
#include <PhoenixSimulator.h>
#endif

#ifndef SIM
#include "MessageDestDS.h"
#endif

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
	namespace phoenix {
		
		void Phoenix::RobotInit() {
			//
			// Initialize message logger
			//
			initializeMessageLogger();

			readParamsFile("phoenix/robot.dat") ;
			
			//
			// This is where the subsystems for the robot get created
			//
			std::shared_ptr<TankDrive> db_p = std::make_shared<TankDrive>(*this, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;
			addSubsystem(db_p) ;

			//
			// Add in the wings subsystem
			//
			auto wings_p = std::make_shared<Wings>(*this) ;
			addSubsystem(wings_p) ;

			//
			// Add in the intake subsystem
			//
			auto intake_p = std::make_shared<Intake>(*this) ;
			intake_ = intake_p; 
			addSubsystem(intake_p) ;

			//
			// This is where we would add the other subsystems for the robot
			//
		}

		std::shared_ptr<ControllerBase> Phoenix::createAutoController() {
			//
			// This is where the autonomous controller is created
			//
			auto intakedutyaction_p = std::make_shared<IntakeDutyCycleAction>(*intake_.get(), .5) ;
			auto phoenixauto_p = std::make_shared<PhoenixAutoController>(intakedutyaction_p, *this);
			return phoenixauto_p;
		}
		
		std::shared_ptr<ControllerBase> Phoenix::createTeleopController() {
			//
			// This is where the teleop controller is created
			//
			return nullptr ;
		}
		 
		std::shared_ptr<ControllerBase> Phoenix::createTestController() {
			//
			// This is where the test controller is created
			//
			return nullptr ;
		}

		void Phoenix::initializeMessageLogger() {
            MessageLogger& logger = getMessageLogger();

			//
			// Enable message of all severities
			//
            logger.enableType(MessageLogger::MessageType::error);
            logger.enableType(MessageLogger::MessageType::warning);
            logger.enableType(MessageLogger::MessageType::info);
            logger.enableType(MessageLogger::MessageType::debug);

            //
            // Decide what message groups (incl. subsystems) you want to see
            //
            logger.enableSubsystem(MSG_GROUP_DRIVEBASE);

			// Set up message logger destination(s)
            std::shared_ptr<MessageLoggerDest> dest_p ;

#if defined(SIM) && !defined(XEROSCREEN)
			//
			// We only want printouts on COUT when we are debugging
			// In competition mode, this information goes to a log file on
			// the USB stick.
			//
			dest_p = std::make_shared<MessageDestStream>(std::cout) ;
			logger.addDestination(dest_p) ;
#endif

			//
			// This is where the roborio places the first USB flash drive it
			// finds.  Other drives are placed at /V, /W, /X.  The devices are
			// actually mounted at /media/sd*, and a symbolic link is created
			// to /U.
			//
#ifndef SIM
			std::string flashdrive("/u/") ;
			std::string logname("logfile_") ;
			dest_p = std::make_shared<MessageDestSeqFile>(flashdrive, logname) ;
			logger.addDestination(dest_p) ;
#endif

			//
			// Send warnings and errors to the driver station
			//
#ifndef SIM
			dest_p = std::make_shared<MessageDestDS>() ;
			logger.addDestination(dest_p) ;
#endif

		}
	}
}

#ifdef SIM
xero::sim::phoenix::PhoenixSimulator sim("phoenix/sim.dat") ;
#endif

//
// This macro call is defined by the WPI library.  It is the call that connects
// the class xero::phoenix::Phoenix to physical robot
//
START_ROBOT_CLASS(xero::phoenix::Phoenix) ;
