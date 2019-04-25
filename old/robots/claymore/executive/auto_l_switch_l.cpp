#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Left
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
	AUTO_PREAMBLE("left_switch_left"),
    startLifterExch,
    Step(Drive("auto:l_switch_l:segment1", 148, false)),
    rotate90pos,
    startLifterSwitch,
    Step(Wait(0.5)),
    Step(Drive("auto:l_switch_l:segment2", 18, true)),
	lifterToSwitch,
    eject,
	startLifterExch,
    Step(Drive("auto:l_switch_l:segment3", -18, true)),
	rotate90neg,
    Step(Drive("auto:l_switch_l:segment4", 36, true)), 
    rotate90pos,
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive left_switch_left(chain) ;
