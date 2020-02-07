#include "DroidFiveBallAutomode.h"

namespace xero
{
    namespace droid
    {
        DroidFiveBallAutomode::DroidFiveBallAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "FarSideFive", "Start on far side, collect two, score five")
        {
        }

        DroidFiveBallAutomode::~DroidFiveBallAutomode()
        {

        }
    }
}