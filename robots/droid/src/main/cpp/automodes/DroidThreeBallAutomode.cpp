#include "DroidThreeBallAutomode.h"

namespace xero
{
    namespace droid
    {
        DroidThreeBallAutomode::DroidThreeBallAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "CenterThree", "Start in center and scrore the three balls")
        {
        }

        DroidThreeBallAutomode::~DroidThreeBallAutomode()
        {

        }
    }
}