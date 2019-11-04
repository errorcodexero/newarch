#include "$$RobotName$$AutoModeBase.h"
#include "$$RobotName$$.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace $$robotname$$ {
        $$RobotName$$AutoModeBase::$$RobotName$$AutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        $$RobotName$$AutoModeBase::~$$RobotName$$AutoModeBase() 
        {            
        }
    }
}