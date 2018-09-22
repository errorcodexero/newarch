#include "OISubsystem.h"
#include "Robot.h"
#include "ActionSequence.h"
#include "tankdrive/TankDrive.h"
#include "basegroups.h"
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
    namespace base {
        OISubsystem::OISubsystem(Robot &robot, const std::string &name, bool adddriver) : Subsystem(robot, name) {
            inited_ = false ;
            enabled_ = false ;
			if (adddriver) {
				int driver = robot.getSettingsParser().getInteger("hw:driverstation:hid:driver") ;                
				driver_ = std::make_shared<DriverGamepad>(*this, driver);
				addHIDDevice(driver_) ;
			}

            seq_ = std::make_shared<ActionSequence>("teleop", robot.getMessageLogger()) ;
        }

        OISubsystem::~OISubsystem() {
        }     

        int OISubsystem::getAutoModeSelector() {
            for(auto dev: hiddevices_) {
                int automode = dev->getAutoModeSelector() ;
                if (automode != -1)
                    return automode ;
            }

            return -1 ;
        }

        void OISubsystem::computeState() {
            if (!inited_) {
                auto sub = getRobot().getDriveBase() ;
                std::shared_ptr<TankDrive> db = std::dynamic_pointer_cast<TankDrive>(sub) ;
                assert(db != nullptr) ; 
                driver_->init(db) ;
                inited_ = true ;
            }

            seq_->clear() ;
            for(auto dev: hiddevices_)
                dev->computeState(*seq_) ;
        }

        void OISubsystem::run() {
            if (enabled_ && seq_->size() > 0) {
                seq_->start() ;
                seq_->run() ;
                if (!seq_->isDone()) {
                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "telop sequence did not complete in one cycle" ;
                    logger.endMessage() ;
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "Sequence: " << seq_->toString() ;
                    logger.endMessage() ;
                }
            }
        }
    }
}
