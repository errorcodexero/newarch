#include <iostream>
#include "Swerve.h"

#ifdef SIMULATOR
#include <SwerveSimulator.h>
xero::sim::swerve::SwerveSimulator sim("robotsims/swervesim/sim.dat") ;
#endif

int main(int ac, char **av)
{

#ifdef SIMULATOR
    frc::StartRobot<xero::swerve::Swerve>(ac, av) ;
#else
    frc::StartRobot<xero::swerve::Swerve>() ;
#endif
    return 0 ;
}
