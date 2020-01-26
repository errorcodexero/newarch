#include <iostream>
#include "Phoenix.h"

#ifdef SIM2
#include <SimulatorEngine.h>
#include <ModelFactory.h>
#include <BuiltInModels.h>

using namespace xero::sim2;
using namespace xero::models;

//
// Specify the simulation models needed by this robot.  These may be in the
// models library or may be part of this code base
//
USE_SIMULATION_MODEL(WestCoastDrive, "westcoastdrive");
USE_SIMULATION_MODEL(NavXSim, "navxsim");
USE_SIMULATION_MODEL(FMS, "fms") ;
USE_SIMULATION_MODEL(DriverGamePad, "drivergamepad") ;
#endif

int main(int ac, char **av)
{
#ifdef SIM2
    xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
    if (engine.start(ac, av) != xero::sim2::SimulatorEngine::ErrorCode::NoError)
        return 1;
#endif

#ifdef SIMULATOR
    frc::StartRobot<xero::phoenix::Phoenix>(ac, av) ;
#else
    frc::StartRobot<xero::phoenix::Phoenix>() ;
#endif
    return 0 ;
}
