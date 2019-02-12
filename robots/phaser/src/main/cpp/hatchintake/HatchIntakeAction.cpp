#include "HatchIntakeAction.h"
#include "HatchIntake.h"
#include "Robot.h"
#include <string>

using namespace xero::misc ;


namespace xero {
    namespace phaser {
            void HatchIntakeAction::start() {
                if (is_deployed_ == true){
                    subsystem_.deployCollector() ;
                    subsystem_.setMotor(collect_power_) ;
                }
                else {
                    subsystem_.retractCollector() ;
                    subsystem_.setMotor(0.0) ;
                }
                is_done_ = true ;
            }

            void HatchIntakeAction::run() {
            
            }

            bool HatchIntakeAction::isDone() {
                return is_done_ ;
            }

            void HatchIntakeAction::cancel() {

            }

            std::string HatchIntakeAction::toString() {
                std::string result = "HatchIntakeAction " ;
                result += std::to_string(is_deployed_) ;
            
                return result ;
            }    
        
    }
}


