#include "AutoMode.h"
#include "basegroups.h"
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {

        AutoMode::AutoMode(Robot &robot, const std::string &name, const std::string &desc) : ActionSequence(robot.getMessageLogger()), robot_(robot)
        {
            name_ = name ;
            desc_ = desc ;
        }

        AutoMode::~AutoMode()
        {            
        }

        std::string AutoMode::toString()
        {
            std::string result ;
            result += name_;
            result += ActionSequence::toString() ;
            return result ;
        }

    }
}