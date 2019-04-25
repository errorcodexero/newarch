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
    Step(StartAuto("rotate_pos90")),
    calibrateLifter,
    calibrateGrabber,
	lifterToExch,
    rotate90pos,
    endAuto
} ;

static Chain chain(steps, teleopex) ;

Executive rotate_pos90(chain) ;
