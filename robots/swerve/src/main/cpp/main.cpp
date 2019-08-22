#include <iostream>
#include "Phaser.h"

#ifdef SIMULATOR
#include <PhaserSimulator.h>
xero::sim::phaser::PhaserSimulator sim("robotsims/phasersim/sim.dat") ;
#endif

int main(int ac, char **av)
{

#ifdef SIMULATOR
    frc::StartRobot<xero::phaser::Phaser>(ac, av) ;
#else
    frc::StartRobot<xero::phaser::Phaser>() ;
#endif
    return 0 ;
}