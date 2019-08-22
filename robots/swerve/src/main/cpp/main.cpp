#include <iostream>
#include "SwerveRobot.h"

#ifdef SIMULATOR
#include <SwerveSimulator.h>
xero::sim::swerve::SwerveSimulator sim("robotsims/swervesim/sim.dat") ;
#endif

int main(int ac, char **av)
{
#ifdef SIMULATOR
    frc::StartRobot<xero::swerve::SwerveRobot>(ac, av) ;
#else
    frc::StartRobot<xero::swerve::SwerveRobot>() ;
#endif
    return 0 ;
}