#include "PhoenixAutoMode.h"

namespace xero
{
    namespace phoenix
    {
        PhoenixAutoMode::PhoenixAutoMode(xero::base::Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {
        }

        PhoenixAutoMode::~PhoenixAutoMode()
        {            
        }
    }
}