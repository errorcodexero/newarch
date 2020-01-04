#include "DroidAutoMode.h"

namespace xero
{
    namespace droid
    {
        DroidAutoMode::DroidAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {
        }

        DroidAutoMode::~DroidAutoMode()
        {            
        }
    }
}