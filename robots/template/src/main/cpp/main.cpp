#include <iostream>
#include "$$RobotName$$.h"

#ifdef SIMULATOR
#include <$$RobotName$$Simulator.h>
xero::sim::$$robotname$$::$$RobotName$$Simulator sim("robotsims/$$robotname$$sim/sim.dat") ;
#endif

int main(int ac, char **av)
{

#ifdef SIMULATOR
    frc::StartRobot<xero::$$robotname$$::$$RobotName$$>(ac, av) ;
#else
    frc::StartRobot<xero::$$robotname$$::$$RobotName$$>() ;
#endif
    return 0 ;
}
