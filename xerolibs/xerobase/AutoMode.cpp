#include "AutoMode.h"
#include "basegroups.h"
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {
        AutoMode::AutoMode(Robot &robot, const std::string &name, const std::string &desc) : SequenceAction(robot.getMessageLogger()), robot_(robot)
        {
            name_ = name ;
            desc_ = desc ;
        }

        AutoMode::~AutoMode()
        {            
        }
    }
}