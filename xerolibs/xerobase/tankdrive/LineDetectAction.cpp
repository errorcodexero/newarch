#include "LineDetectAction.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
            LineDetectAction::LineDetectAction(LightSensorSubsystem &ls_subsystem, TankDrive &db_subsystem) : TankDriveAction(db_subsystem), ls_subsystem_(ls_subsystem) {
            }

            LineDetectAction::~LineDetectAction() {                
            }

            void LineDetectAction::start() {
            }

            void LineDetectAction::run() {
                is_done_ = ls_subsystem_.detectedObject() ;
            }

            bool LineDetectAction::isDone() {
                return is_done_ ;
            }

            void LineDetectAction::cancel() {
            }

            std::string LineDetectAction::toString() {
                std::string result = "LineDetectAction " ;
                return result ;
            }
    }
}
