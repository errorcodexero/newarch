#include "autosteps.h"
#include "chain.h"
#include <vector>

using namespace std ;

////////////////////   Calibrate Only Auto Program /////////////////////////////////

static vector<Step> steps =
{
    AUTO_PREAMBLE("calibrate_only"),
	AUTO_POSTAMBLE
} ;

static Chain chain(steps, teleopex) ;

Executive calibrate_only(chain) ;

