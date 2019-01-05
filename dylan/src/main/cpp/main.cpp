#include <iostream>
#include "Dylan.h"

#ifdef SIMULATOR
xero::sim::dylan::DylanSimulator sim("dylan/sim.dat") ;
#endif

int main(int ac, char **av)
{
    frc::StartRobot<xero::dylan::Dylan>() ;
    return 0 ;
}