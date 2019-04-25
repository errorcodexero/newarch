#include "autosteps.h"
#include "chain.h"
#include <vector>


using namespace std ;

//////////////////////////////////////////////////////////////////////////////////////
//
// Start: Right
// Action: Score on right scale
//
///////////////////////////////////////////////////////////////////////////////////////
static vector<Step> steps =
{
    AUTO_PREAMBLE("right_scale_right"),
#ifdef SAFE_TWO_CUBE
	Step(Drive(290, false)),
	Step(Rotate(-90, 8.0)),
    Step(Lifter_to_height(85.0)),
    eject,
	lifterToFloor,
    Step(Rotate_finish(-90, -67, 3.0)),
	Step(Drive_and_collect(114.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToExch,
	Step(Drive(true, -30)),
	Step(Rotate_back(0.0, 10.0)),
	lifterToScale,
    eject,
	lifterToExch,
#else
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(253.0, 100.0, -30.0, false)),
    Step(Lifter_to_height(85.0)),
	Step(Eject(0.7)),
	lifterToFloor,
	Step(Rotate(-115)),
	Step(Drive_and_collect(56.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(true, -6.0)),
	Step(Rotate_back(-25.0, 10.0)),
    Step(Lifter_to_height(85.0)),
	eject,
	lifterToExch,
#endif
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive right_scale_right(chain) ;

static vector<Step> steps_switch =
{
    AUTO_PREAMBLE("right_scale_right_switch_right"),
#ifdef SAFE_TWO_CUBE
	Step(Drive(290, false)),
	Step(Rotate(-90, 8.0)),
    Step(Lifter_to_height(85.0)),
    eject,
	lifterToFloor,
    Step(Rotate_finish(-90, -67, 3.0)),
	Step(Drive_and_collect(114.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToSwitch,
	Step(Drive(12.0)),
    eject,
	Step(Drive(-6.0)),
	lifterToExch,
#else
	Step(Background_lifter_to_preset(85.0, 0.0)),
	Step(Drive(265.0, 120.0, -25.0, false)),
    Step(Lifter_to_height(85.0)),
	Step(Eject(0.7)),
	lifterToFloor,
	Step(Rotate(-107)),
	Step(Drive_and_collect(63.0)),
	Step(Close_collect_no_cube(1.0)),
	Step(Drive(-6.0)),
	Step(Close_collect_no_cube(0.5)),
	lifterToSwitch,
	Step(Drive(24.0)),
    eject,
	Step(Drive(-6.0)),
	lifterToExch,
#endif
	AUTO_POSTAMBLE,
} ;

static Chain chain_switch(steps_switch, teleopex) ;
Executive right_scale_right_switch_right(chain_switch) ;
