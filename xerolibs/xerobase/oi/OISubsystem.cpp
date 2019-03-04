#include "oi/OISubsystem.h"
#include "Robot.h"
#include "ActionSequence.h"
#include "tankdrive/TankDrive.h"
#include "basegroups.h"
#include "DriverGamepadRumbleAction.h"
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
    namespace base {
        OISubsystem::OISubsystem(Robot &robot, const std::string &name, bool adddriver) 
                                : Subsystem(robot, name) {
            inited_ = false ;
            if (adddriver) {
                int driver = robot.getSettingsParser().getInteger("hw:driverstation:hid:driver") ;                
                driver_ = std::make_shared<DriverGamepad>(*this, driver);
                addHIDDevice(driver_) ;
            }

            std::string sequence_name = "OIsubsystem";
        }

        OISubsystem::~OISubsystem() {
        }     

        void OISubsystem::postHWInit() {
            Subsystem::postHWInit() ;
            for(auto dev: hiddevices_)
                dev->init() ;            
        }

        int OISubsystem::getAutoModeSelector() {
            for(auto dev: hiddevices_) {
                int automode = dev->getAutoModeSelector() ;
                if (automode != -1)
                    return automode  ;
            }

            return -1 ;
        }

        void OISubsystem::computeState() {
            if (!inited_) {
                auto sub = getRobot().getDriveBase() ;
                std::shared_ptr<TankDrive> db = std::dynamic_pointer_cast<TankDrive>(sub) ;
                if (db != nullptr) {
                    driver_->init(db) ;
                    inited_ = true ;
                }
                else {
                    std::cout << "We got a null db" << std::endl ;
                    if (sub != nullptr)
                        std::cout << "But it was a " << sub->getName() << std::endl ;
                }
            }
            for(auto dev: hiddevices_)
                dev->computeState() ;
        }

        void OISubsystem::generateActions(ActionSequencePtr seq) {
            for(auto dev: hiddevices_)
            {
                if (dev->isEnabled())
                    dev->generateActions(*seq) ;
            }
        }

        void OISubsystem::run() {
        }

        bool OISubsystem::canAcceptAction(ActionPtr act) {
            std::shared_ptr<DriverGamepadRumbleAction> dact = std::dynamic_pointer_cast<DriverGamepadRumbleAction>(act) ;
            return dact != nullptr ;
        }
    }
}
