
#include "Bunny.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include "automodes/BunnyAutoMode.h"
#include <ActionSequence.h>
#include <basegroups.h>
#include <DelayAction.h>
#include <MessageDestDS.h>
#include <MessageLogger.h>
#include <MessageDestSeqFile.h>
#include <MessageDestStream.h>
#include <SmartDashboard/SmartDashboard.h>

#ifdef SIM
#include <Bunny2018Simulator.h>
#endif

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
	namespace bunny2018 {

		Bunny::Bunny() : xero::base::Robot(0.02) {
		}

		std::shared_ptr<BunnySubsystem> Bunny::getBunnySubsystem() {
			auto sub = getRobotSubsystem() ;
			return std::dynamic_pointer_cast<BunnySubsystem>(sub) ;
		}

		void Bunny::DoDisabledWork() {
			auto oi = getBunnySubsystem()->getOI() ;			
			int automode = oi->getAutoModeSelector() ;
			std::string autostr = std::to_string(automode) ;
			frc::SmartDashboard::PutString("AutoModeNumber", autostr) ;
		}
		
		void Bunny::RobotInit() {
			std::string filename ;

			//
			// Initialize message logger
			//
			initializeMessageLogger();

			auto &logger = getMessageLogger() ;
			logger.startMessage(MessageLogger::MessageType::info) ;
			logger << "Initializing Bunny2018 Robot" ;
			logger.endMessage() ;

#ifdef SIM
			filename = "bunny2018/robot.dat" ;
#else
			filename = "/home/lvuser/robot.dat" ;
#endif

			if (!readParamsFile(filename)) {
				std::cerr << "Robot  Initialization failed - could not read robot data file '" ;
				std::cerr << filename << "'" << std::endl ;
				assert(false) ;
			}
			
			//
			// This is where the subsystems for the robot get created
			//
			auto robot_p = std::make_shared<BunnySubsystem>(*this) ;
			SubsystemPtr db = robot_p->getDriveBase() ;
			SubsystemPtr oi = robot_p->getOI() ;
			setRobotSubsystem(robot_p, oi, db) ;

			logger.startMessage(MessageLogger::MessageType::info) ;
			logger << "Initialization complete." ;
			logger.endMessage() ;			
		}

		std::shared_ptr<ControllerBase> Bunny::createAutoController() {
			return std::make_shared<BunnyAutoMode>(*this) ;
		}
		
		std::shared_ptr<ControllerBase> Bunny::createTeleopController() {
			//
			// This is where the teleop controller is created
			//
			return nullptr ;
		}
		 
		std::shared_ptr<ControllerBase> Bunny::createTestController() {
			//
			// This is where the test controller is created
			//
			return nullptr ;
		}

		void Bunny::initializeMessageLogger() {
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
			// logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
			logger.enableSubsystem(MSG_GROUP_ACTIONS);
			//logger.enableSubsystem(MSG_GROUP_PARSER) ;
			logger.enableSubsystem(MSG_GROUP_OI) ;

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
xero::sim::bunny2018::Bunny2018Simulator sim("bunny2018/sim.dat") ;
#endif

//
// This macro call is defined by the WPI library.  It is the call that connects
// the class xero::Bunny::Phoenix to physical robot
//
START_ROBOT_CLASS(xero::bunny2018::Bunny) ;
