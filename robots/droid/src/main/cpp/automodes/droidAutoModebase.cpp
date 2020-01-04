#include "droidAutoModeBase.h"
#include "droid.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {
        droidAutoModeBase::droidAutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        droidAutoModeBase::~droidAutoModeBase() 
        {            
        }
    }
}
