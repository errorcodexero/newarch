#include "CargoIntakeAction.h"
#include "CargoIntake.h"
#include "Robot.h"
#include <string>

using namespace xero::misc ;


namespace xero {
    namespace phaser {
            void CargoIntakeAction::start() {
                if (is_deployed_ == true){
                    subsystem_.deployCollector() ;
                }
                else {
                    subsystem_.retractCollector() ;
                    subsystem_.setMotor(0.0) ;
                }
                is_done_ = true ;
            }

            void CargoIntakeAction::run() {
            
            }

            bool CargoIntakeAction::isDone() {
                return is_done_ ;
            }

            void CargoIntakeAction::cancel() {

            }

            std::string CargoIntakeAction::toString() {
                std::string result = "CargoIntakeAction " ;
                result += std::to_string(is_deployed_) ;
            
                return result ;
            }    
        
    }
}


