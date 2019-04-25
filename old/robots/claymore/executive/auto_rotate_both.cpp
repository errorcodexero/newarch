#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on right switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    Step(StartAuto("rotate_both")),
    calibrateLifter,
    calibrateGrabber,
	lifterToExch,
    rotate90pos,
    Step(Wait(2.0)),
    rotate90neg,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive rotate_both(chain) ;
