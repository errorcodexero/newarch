#include "DroidEightBallAutomode.h"

namespace xero
{
    namespace droid
    {
        DroidEightBallAutomode::DroidEightBallAutomode(xero::base::Robot &robot) : 
            DroidAutoMode(robot, "NearSideEight", "Start on near side, score three, collect five, score five")
        {
        }

        DroidEightBallAutomode::~DroidEightBallAutomode()
        {

        }
    }
}