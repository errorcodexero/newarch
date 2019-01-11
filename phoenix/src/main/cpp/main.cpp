#include <iostream>
#include "Phoenix.h"

#ifdef SIMULATOR
xero::sim::phoenix::PhoenixSimulator sim("phoenix/sim.dat") ;
#endif

int main(int ac, char **av)
{
    frc::StartRobot<xero::phoenix::Phoenix>() ;
    return 0 ;
}