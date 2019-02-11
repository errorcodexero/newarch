#include "CargoHolderAction.h"
#include "CargoHolder.h"
#include "Robot.h"

using namespace xero::misc ;


namespace xero {
    namespace phaser {
            void CargoHolderAction::start() {
                subsystem_.setMotor(power_) ; 
                is_done_ = true ; 
            }

            void CargoHolderAction::run() {
            }

            bool CargoHolderAction::isDone() {
                return is_done_ ;
            }

            void CargoHolderAction::cancel() {
                is_done_ = true ; 
            }

            std::string CargoHolderAction::toString() {
                std::string result = "CargoHolderAction " ;
                result += std::to_string(power_) ;
                return result ;
            }
    }
}
