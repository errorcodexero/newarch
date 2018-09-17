
#include "GoPiGo3Xero.h"
#include "gopigo3groups.h"
#include "GoPiGo3AutoModeController.h"
#include "GoPiGo3Subsystem.h"
#include <ActionSequence.h>
#include <basegroups.h>
#include <DelayAction.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
	namespace gopigo {

		std::shared_ptr<GoPiGo3Subsystem> GoPiGo3Xero::getRobotSubsystem() {
			auto sub = getSubsystemByName("gopigo") ;
			return std::dynamic_pointer_cast<GoPiGo3Subsystem>(sub) ;
		}
		
		void GoPiGo3Xero::RobotInit() {
			std::string filename ;

			//
			// Initialize message logger
			//
			initializeMessageLogger();

			//
			// The parameters file for the robot
			//
			filename = "/home/pi/robot.dat" ;

			if (!readParamsFile(filename)) {
				std::cerr << "Rboto Initialization failed - could not read robot data file '" ;
				std::cerr << filename << "'" << std::endl ;
				assert(false) ;
			}
			
			//
			// Create the subsystem for the robot
			//
			auto sub = std::make_shared<GoPiGo3Subsystem>(*this) ;
			addSubsystem(sub) ;
		}

		std::shared_ptr<ControllerBase> GoPiGo3Xero::createAutoController() {
			return std::make_shared<GoPiGo3AutoModeController>(*this) ;
		}
		
		std::shared_ptr<ControllerBase> GoPiGo3Xero::createTeleopController() {
			//
			// This is where the teleop controller is created
			//
			return nullptr ;
		}
		 
		std::shared_ptr<ControllerBase> GoPiGo3Xero::createTestController() {
			//
			// This is where the test controller is created
			//
			return nullptr ;
		}

		void GoPiGo3Xero::initializeMessageLogger() {
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
			logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
			logger.enableSubsystem(MSG_GROUP_ACTIONS) ;

			// Set up message logger destination(s)
            std::shared_ptr<MessageLoggerDest> dest_p ;

			//
			// This is where the roborio places the first USB flash drive it
			// finds.  Other drives are placed at /V, /W, /X.  The devices are
			// actually mounted at /media/sd*, and a symbolic link is created
			// to /U.
			//
			std::string flashdrive("/home/pi/logs");
			std::string logname("logfile_");
			dest_p = std::make_shared<MessageDestSeqFile>(flashdrive, logname);
			logger.addDestination(dest_p);

#ifdef DEBUG
			dest_p = std::make_shared<MessageDestStream>(std::cout);
			logger.addDestination(dest_p);
#endif

			//
			// Send warnings and errors to the driver station
			//
			dest_p = std::make_shared<MessageDestDS>();
			logger.addDestination(dest_p);
		}
	}
}

//
// This macro call is defined by the WPI library.  It is the call that connects
// the class xero::GoPiGo3Xero::Phoenix to physical robot
//
START_ROBOT_CLASS(xero::gopigo::GoPiGo3Xero) ;
