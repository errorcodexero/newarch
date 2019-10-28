#include <iostream>
#include "Ranseur.h"

#ifdef SIMULATOR
#include <ranseurSimulator.h>
xero::sim::ranseur::ranseurSimulator sim("robotsims/ranseursim/sim.dat") ;
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
