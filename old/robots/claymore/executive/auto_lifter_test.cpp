#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps
{
    Step(StartAuto("lifter_test")),
    calibrateLifter,
    startLifterSwitch,
    Step(Drive(50.0, false)),
	lifterToSwitch,
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive lifter_test(chain) ;
