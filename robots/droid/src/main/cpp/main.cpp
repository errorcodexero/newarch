#include <iostream>
#include "droid.h"

#ifdef SIMULATOR
#include <droidSimulator.h>
xero::sim::droid::droidSimulator sim("robotsims/droidsim/sim.dat") ;
#endif

int main(int ac, char **av)
{

#ifdef SIMULATOR
    frc::StartRobot<xero::droid::droid>(ac, av) ;
#else
    frc::StartRobot<xero::droid::droid>() ;
#endif
    return 0 ;
}
