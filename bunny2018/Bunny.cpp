
#include "Bunny.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "bunnyoi/BunnyOISubsystem.h"
#include "automodes/BunnyAutoMode.h"
#include "bunnyids.h"
#include <ActionSequence.h>
#include <basegroups.h>
#include <DelayAction.h>
#include <MessageLogger.h>
#include <SmartDashboard/SmartDashboard.h>

#ifdef ENABLE_SIMULATOR
#include <Bunny2018Simulator.h>
#endif

#include <memory>
#include <cassert>

using namespace xero::misc ;
using namespace xero::base ;

namespace xero {
	namespace bunny2018 {

		//
		// Note, this name (bunny2018) must match the name where the source code is
		// located.  The robot data file will be found in that directory for simulation
		// and will be named with the give name with a dat extension.  For instance, for
		// bunny2018, the file will be bunny2018/bunny2018.dat
		//
		Bunny::Bunny() : xero::base::Robot("bunny2018", 0.02) {
		}

		std::shared_ptr<BunnySubsystem> Bunny::getBunnySubsystem() {
			auto sub = getRobotSubsystem() ;
			return std::dynamic_pointer_cast<BunnySubsystem>(sub) ;
		}

		void Bunny::enableSpecificMessages() {
            MessageLogger& logger = getMessageLogger();

            //
            // Decide what message groups (incl. subsystems) you want to see
            //
			// logger.enableSubsystem(MSG_GROUP_TANKDRIVE);
			// logger.enableSubsystem(MSG_GROUP_ACTIONS);
			// logger.enableSubsystem(MSG_GROUP_PARSER) ;
			// logger.enableSubsystem(MSG_GROUP_OI) ;
			// logger.enableSubsystem(MSG_GROUP_SORTER) ;
			// logger.enableSubsystem(MSG_GROUP_SORTER_VERBOSE) ;
			//
			
            logger.enableSubsystem(MSG_GROUP_ALL) ;			
		}

		void Bunny::RobotHardwareInit() {
			//
			// This is where the subsystems for the robot get created
			//
			auto robot_p = std::make_shared<BunnySubsystem>(*this) ;
			setRobotSubsystem(robot_p, robot_p->getOI(), robot_p->getDriveBase()) ;
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

	}
}

#ifdef ENABLE_SIMULATOR
xero::sim::bunny2018::Bunny2018Simulator sim("bunny2018/sim.dat") ;
#endif

//
// This macro call is defined by the WPI library.  It is the call that connects
// the class xero::Bunny::Phoenix to physical robot
//
START_ROBOT_CLASS(xero::bunny2018::Bunny) ;
