#include "MotorControllerAutoModeBase.h"
#include "MotorController.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace motorcontroller {
        MotorControllerAutoModeBase::MotorControllerAutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        MotorControllerAutoModeBase::~MotorControllerAutoModeBase() 
        {            
        }
    }
}
