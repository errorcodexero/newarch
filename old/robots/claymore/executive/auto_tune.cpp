#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

static vector<Step> steps_right =
{
    Step(StartAuto("tune_square_right")),
    calibrateLifter,
    calibrateGrabber,
	lifterToExch,
	Step(Drive("tune:square", 120, false)),
	Step(Rotate(90.0, 3.0)),
	Step(Drive("tune:square", 120, false)),
	Step(Rotate(90.0, 3.0)),
	Step(Drive("tune:square", 120, false)),
	Step(Rotate(90.0, 3.0)),
	Step(Drive("tune:square", 120, false)),
	Step(Rotate(90.0, 3.0)),
    endAuto
} ;

static Chain chain_right(steps_right, teleopex) ;
Executive square_right(chain_right) ;

static vector<Step> steps_left =
{
    Step(StartAuto("tune_square_left")),
    calibrateLifter,
    calibrateGrabber,
	lifterToExch,
	Step(Drive("tune:square", 120, false)),
	Step(Wait(1.0)),
    rotate90neg,
	Step(Drive("tune:square", 120, false)),
	Step(Wait(1.0)),
    rotate90neg,
	Step(Drive("tune:square", 120, false)),
	Step(Wait(1.0)),
    rotate90neg,
	Step(Drive("tune:square", 120, false)),
	Step(Wait(1.0)),
    rotate90neg,
    endAuto
} ;

static Chain chain_left(steps_left, teleopex) ;
Executive square_left(chain_left) ;
