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
    Step(StartAuto("lifter_move")),
    calibrateLifter,
    Step(Lifter_to_height(24.0)),
    Step(Wait(2.0)),
    Step(Lifter_to_height(43.5)),
    Step(Wait(2.0)),
    Step(Lifter_to_height(24.0)),
    endAuto,
};

static Chain chain(steps, teleopex) ;
Executive lifter_move(chain) ;
