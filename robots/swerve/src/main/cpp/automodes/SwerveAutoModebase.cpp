#include "SwerveAutoModeBase.h"
#include "Swerve.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveFollowPathAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace swerve {
        SwerveAutoModeBase::SwerveAutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        SwerveAutoModeBase::~SwerveAutoModeBase() 
        {            
        }
    }
}
