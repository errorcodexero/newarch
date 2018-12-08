#include "Phoenix.h"
#include "phoenixgroups.h"
#include "PhoenixAutoController.h"

// Subsystems


// Other misc classes
#include <ActionSequence.h>
#include <DelayAction.h>

#include <basegroups.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>

#ifdef SIM
#include <PhoenixSimulator.h>
#endif

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
	namespace phoenix {
		
		void Phoenix::RobotInit() {
			std::string filename ;

			//
			// Initialize message logger
			//
			initializeMessageLogger();

#ifdef SIM
			filename = "phoenix/robot.dat" ;
#else
			filename = "/home/lvuser/robot.dat" ;
#endif

			if (!readParamsFile(filename)) {
				std::cerr << "Rboto Initialization failed - could not read robot data file '" ;
				std::cerr << filename << "'" << std::endl ;
				assert(false) ;
			}

			auto sub_p = std::make_shared<PhoenixRobotSubsystem>(*this) ;
			setRobotSubsystem(sub_p, sub_p->getOI(), sub_p->getTankDrive()) ;
		}

		std::shared_ptr<ControllerBase> Phoenix::createAutoController() {
			return nullptr ;
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
			// logger.enableSubsystem(MSG_GROUP_PATHFOLLOWER) ;
			logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
			logger.enableSubsystem(MSG_GROUP_LIFTER);
			logger.enableSubsystem(MSG_GROUP_GRABBER);
			logger.enableSubsystem(MSG_GROUP_COLLECTOR) ;
			logger.enableSubsystem(MSG_GROUP_ACTIONS);
			logger.enableSubsystem(MSG_GROUP_PARSER) ;


			// Set up message logger destination(s)
            std::shared_ptr<MessageLoggerDest> dest_p ;

#if defined(SIM)
			if (!isScreen())
			{
				dest_p = std::make_shared<MessageDestStream>(std::cout);
				logger.addDestination(dest_p);
			}

			const std::string outfile = getRobotOutputFile();
			if (outfile.length() > 0)
				setupRobotOutputFile(outfile);

#else

			//
			// This is where the roborio places the first USB flash drive it
			// finds.  Other drives are placed at /V, /W, /X.  The devices are
			// actually mounted at /media/sd*, and a symbolic link is created
			// to /U.
			//
			std::string flashdrive("/u/");
			std::string logname("logfile_");
			dest_p = std::make_shared<MessageDestSeqFile>(flashdrive, logname);
			logger.addDestination(dest_p);

#ifdef DEBUG
			dest_p = std::make_shared<MessageDestStream>(std::cout);
			logger.addDestination(dest_p);
#endif

#endif

#ifndef SIM
			//
			// Send warnings and errors to the driver station
			//
			dest_p = std::make_shared<MessageDestDS>();
			logger.addDestination(dest_p);
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
