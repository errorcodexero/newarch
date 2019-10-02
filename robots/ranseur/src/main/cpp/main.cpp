#include <iostream>
#include "Ranseur.h"

#ifdef SIMULATOR
#include <RanseurSimulator.h>
xero::sim::ranseur::RanseurSimulator sim("robotsims/ranseursim/sim.dat") ;
#endif

int main(int ac, char **av)
{

#ifdef SIMULATOR
    frc::StartRobot<xero::ranseur::Ranseur>(ac, av) ;
#else
    frc::StartRobot<xero::ranseur::Ranseur>() ;
#endif
    return 0 ;
}