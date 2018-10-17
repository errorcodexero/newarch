
#include "Phoenix.h"
#include "phoenixgroups.h"
#include "PhoenixAutoController.h"
#include "intake/IntakeDutyCycleAction.h"
#include "grabber/GrabberToAngleAction.h"
#include "grabber/GrabberCalibrateAction.h"
#include "lifter/LifterGoToHeightAction.h"
#include "lifter/LifterCalibrateAction.h"
#include "collector/Collector.h"
#include "collector/CollectorCollectCubeAction.h"
#include <ActionSequence.h>
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
			
			//
			// This is where the subsystems for the robot get created
			//
			auto db_p = std::make_shared<TankDrive>(*this, std::list<int>{1, 2, 3}, std::list<int>{4, 5, 6}) ;
			addSubsystem(db_p) ;

			//
			// Add in the wings subsystem
			//
			auto wings_p = std::make_shared<Wings>(*this) ;
			addSubsystem(wings_p) ;

			//
			// Add in the collector
			//
			auto collector_p = std::make_shared<Collector>(*this) ;
			addSubsystem(collector_p) ;

			//
			// Add in the lifter
			//
			auto lifter_p = std::make_shared<Lifter>(*this) ;
			addSubsystem(lifter_p) ;
		}

		std::shared_ptr<ControllerBase> Phoenix::createAutoController() {
			SubsystemPtr sub_p ;

			//
			// This is where the autonomous controller is created
			//
			auto actlist_p = std::make_shared<ActionSequence>(getMessageLogger()) ;				

			sub_p = getSubsystemByName("Collector") ;
			auto collector_p = std::dynamic_pointer_cast<Collector>(sub_p) ;

			sub_p = getSubsystemByName("lifter") ;
			auto lifter_p = std::dynamic_pointer_cast<Lifter>(sub_p) ;

			actlist_p->pushSubActionPair(collector_p->getGrabber(), std::make_shared<GrabberCalibrateAction>(*collector_p->getGrabber())) ;
			actlist_p->pushSubActionPair(lifter_p, std::make_shared<LifterCalibrateAction>(*lifter_p)) ;
			actlist_p->pushSubActionPair(lifter_p, std::make_shared<LifterGoToHeightAction>(*lifter_p, 11.5), false) ;
			actlist_p->pushSubActionPair(collector_p, std::make_shared<CollectorCollectCubeAction>(*collector_p)) ;

			auto phoenixauto_p = std::make_shared<PhoenixAutoController>(actlist_p, *this);
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
