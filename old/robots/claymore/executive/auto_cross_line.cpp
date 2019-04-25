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
static vector<Step> steps =
{
    AUTO_PREAMBLE("cross_line"),
    Step(Drive("auto:cross_line:segment1",144.0, false)),
	AUTO_POSTAMBLE,
} ;

static Chain chain(steps, teleopex) ;
Executive cross_line(chain) ;
