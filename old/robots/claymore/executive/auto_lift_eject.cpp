#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    Step(StartAuto("lift_eject")),
    calibrateLifter,
    calibrateGrabber,
    Step(Lifter_to_height(89.3)),
    eject,
    lifterToExch,
    endAuto,
} ;

static Chain chain(steps, teleopex) ;
Executive lift_eject(chain) ;
