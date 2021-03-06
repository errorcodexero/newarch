#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Two cube score on right, scale then switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("right_two_cube"),
    startLifterExch,
    Step(Drive("auto:r_scale_r:segment1", 306, false)),
    rotate90neg,
    lifterToScale,
    eject,
    Step(Rotate(-75.0)),						// Would be great to get from parameter
    // open collector
    Step(Drive("auto:r_scale_r:segment2", 100, false)),		// No clue, need to figure out
    // close collector and collect
    lifterToSwitch,
    Step(Rotate(-15.0)),						// Would be great to get from parameter
    Step(Drive("auto:r_scale_r:segment2", 12, true)),		// No clue, need to figure out
	AUTO_POSTAMBLE
} ;

static Chain chain(steps, teleopex) ;
Executive two_cube_right(chain) ;
