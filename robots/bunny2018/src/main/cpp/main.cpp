#include <iostream>
#include "Bunny.h"

#ifdef SIMULATOR
#include <Bunny2018Simulator.h>
xero::sim::bunny2018::Bunny2018Simulator sim("robotsims/bunny2018sim/sim.dat") ;
#endif

int main(int ac, char **av)
{
#ifdef SIMULATOR
    frc::StartRobot<xero::bunny2018::Bunny>(ac, av) ;
#else
    frc::StartRobot<xero::bunny2018::Bunny>() ;
#endif
    return 0 ;
}