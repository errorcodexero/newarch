#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: N/A
// Action: Test lifter failure
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps
{
    Step(StartAuto("lifter_fail_test")),
	calibrateLifter,
	lifterToExchFail,
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive lifter_fail_test(chain) ;
