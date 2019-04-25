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
    Step(StartAuto("rotate_neg90")),
    calibrateLifter,
    calibrateGrabber,
	lifterToExch,
    rotate90neg,
    endAuto
} ;

static Chain chain(steps, teleopex) ;
Executive rotate_neg90(chain) ;
