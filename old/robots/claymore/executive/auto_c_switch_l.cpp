#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

static constexpr double angletol = 8.0 ;

///////////////////////////////////////////////////////////////////////////////////////
//
// Start: Center
// Action: Score on left switch
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> two_switch_steps
{
	AUTO_PREAMBLE_LOWER("center_switch_left_two_switch"),
    Step(Drive("auto:c_switch_l:segment1", 52.0, false)),
	Step(Rotate(-90,angletol)),
	Step(Drive("auto:c_switch_l:segment2", 100.0, false)),
	Step(Rotate(90,angletol)),
    startLifterSwitch,
    Step(Drive("auto:c_switch_l:segment3", 56.0, true)),
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_l:segment4", -24, false)),
	lifterToExch,
	Step(Rotate(90, angletol, true)),
	lifterToFloor,
	Step(Drive_and_collect(170.0)),
	lifterToExch,
	AUTO_POSTAMBLE,
};

static Chain two_switch_chain(two_switch_steps, teleopex) ;
Executive center_switch_left_two(two_switch_chain) ;

static vector<Step> scale_right
{
	AUTO_PREAMBLE_LOWER("center_switch_left_scale_right"),
    Step(Drive("auto:c_switch_l:segment1", 52.0, false)),
	Step(Rotate(-90,angletol)),
	Step(Drive("auto:c_switch_l:segment2", 100.0, false)),
	Step(Rotate(90,angletol)),
    startLifterSwitch,
    Step(Drive("auto:c_switch_l:segment3", 56.0, true)),
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_l:segment4", -24, false)),
	lifterToExch,
	Step(Rotate(90, angletol, true)),
	lifterToFloor,
	Step(Drive_and_collect(170.0)),
	lifterToExch,
	Step(Drive(true, -160.0)),
	Step(Rotate(-90,angletol)),
	AUTO_POSTAMBLE,
};

static Chain scale_right_chain(scale_right, teleopex) ;
Executive center_switch_left_scale_right(scale_right_chain) ;

static vector<Step> scale_left
{
	AUTO_PREAMBLE_LOWER("center_switch_left_scale_left"),
    Step(Drive("auto:c_switch_l:segment1", 52.0, false)),
	Step(Rotate(-90,angletol)),
	Step(Drive("auto:c_switch_l:segment2", 100.0, false)),
	Step(Rotate(90,angletol)),
    startLifterSwitch,
    Step(Drive("auto:c_switch_l:segment3", 56.0, true)),
	lifterToSwitch,
    eject,
    Step(Drive("auto:c_switch_l:segment4", -24, false)),
	lifterToExch,
	Step(Rotate(90, angletol, true)),
	lifterToFloor,
	Step(Drive_and_collect(170.0)),
	lifterToExch,
	Step(Drive(true, 52.0)),
	Step(Rotate(-90,angletol)),
	AUTO_POSTAMBLE,
};

static Chain scale_left_chain(scale_left, teleopex) ;
Executive center_switch_left_scale_left(scale_left_chain) ;
