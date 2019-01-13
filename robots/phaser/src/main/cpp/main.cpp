#include <iostream>
#include "Phaser.h"

#ifdef SIMULATOR
xero::sim::phaser::PhaserSimulator sim("phaser/sim.dat") ;
#endif

int main(int ac, char **av)
{
    frc::StartRobot<xero::phaser::Phaser>() ;
    return 0 ;
}