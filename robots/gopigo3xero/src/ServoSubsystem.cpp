#include "ServoSubsystem.h"
#include "ServoAction.h"
#include <Robot.h>
#include <SettingsParser.h>

using namespace xero::base ;

namespace xero {
    namespace gopigo {
        ServoSubsystem::ServoSubsystem(Robot &robot) : Subsystem(robot, "servo") {
            int which = robot.getSettingsParser().getDouble("hw:servo:which") ;
            servo_ = std::make_shared<frc::Servo>(which) ;
        }

        ServoSubsystem::~ServoSubsystem() {         
        }

        bool ServoSubsystem::canAcceptAction(ActionPtr action) {
            auto act = std::dynamic_pointer_cast<ServoAction>(action) ;
            return act != nullptr ;
        }
    }
}
