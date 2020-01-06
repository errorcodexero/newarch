#include <iostream>
#include "MotorController.h"

#ifdef SIMULATOR
#include <MotorControllerSimulator.h>
xero::sim::motorcontroller::MotorControllerSimulator sim("robotsims/motorcontrollersim/sim.dat") ;
#endif

int main(int ac, char **av)
{

#ifdef SIMULATOR
    frc::StartRobot<xero::motorcontroller::MotorController>(ac, av) ;
#else
    frc::StartRobot<xero::motorcontroller::MotorController>() ;
#endif
    return 0 ;
}
