#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: N/A
// Action: Test curved driving
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps
{
    Step(StartAuto("curve_test")),
	Step(Drive(120.0, 30.0, 0.0, false)),
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive curve_test(chain) ;
