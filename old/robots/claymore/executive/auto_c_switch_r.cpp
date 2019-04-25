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
static vector<Step> two_switch_steps =
{
    AUTO_PREAMBLE_LOWER("center_switch_right_two_switch"),
	startLifterSwitch,
    Step(Drive("auto:c_switch_r:segment1", 105, true)),
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_r:segment2", -24, false)),
	lifterToExch,
	Step(Rotate(-90.0, true)),
	lifterToFloor,
	Step(Drive_and_collect(200.0)),
	lifterToExch,
	Step(Drive(true, -10)),
	Step(Rotate(90.0)),
	startLifterSwitch,
	Step(Drive("auto:c_switch_r:segment3", 24, false)),
	lifterToSwitch,
	eject,
	Step(Drive("auto_c_switch_r:segment4", -8, true)),
	startLifterFloor,
	Step(Rotate(-90.0)),
	lifterToFloor,
	AUTO_POSTAMBLE,
} ;

static Chain two_switch_chain(two_switch_steps, teleopex) ;
Executive center_switch_right_two(two_switch_chain) ;

static vector<Step> scale_right =
{
    AUTO_PREAMBLE_LOWER("center_switch_right_scale_right"),
	startLifterSwitch,
    Step(Drive("auto:c_switch_r:segment1", 105, true)),
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_r:segment2", -24, false)),
	lifterToExch,
	Step(Rotate(-90.0, true)),
	lifterToFloor,
	Step(Drive_and_collect(200.0)),
	lifterToExch,
	Step(Drive(true, 54.0)),
	Step(Rotate(90.0)),
	AUTO_POSTAMBLE,
} ;
static Chain scale_right_chain(scale_right, teleopex) ;
Executive center_switch_right_scale_right(scale_right_chain) ;

static vector<Step> scale_left =
{
    AUTO_PREAMBLE_LOWER("center_switch_right_scale_left"),
	startLifterSwitch,
    Step(Drive("auto:c_switch_r:segment1", 105, true)),
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_r:segment2", -24, false)),
	lifterToExch,
	Step(Rotate(-90.0, true)),
	lifterToFloor,
	Step(Drive_and_collect(200.0)),
	lifterToExch,
	Step(Drive(true, -160)),
	Step(Rotate(90.0)),
	AUTO_POSTAMBLE,
} ;

static Chain scale_left_chain(scale_left, teleopex) ;
Executive center_switch_right_scale_left(scale_left_chain) ;

