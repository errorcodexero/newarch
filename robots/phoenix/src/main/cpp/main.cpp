#include <iostream>
#include "Phoenix.h"

#ifdef SIMULATOR
#include <PhoenixSimulator.h>
xero::sim::phoenix::PhoenixSimulator sim("robotsims/phoenix/sim.dat") ;
#endif

int main(int ac, char **av)
{
#ifdef SIMULATOR
    frc::StartRobot<xero::phoenix::Phoenix>(ac, av) ;
#else
    frc::StartRobot<xero::phoenix::Phoenix>() ;
#endif
    return 0 ;
}
