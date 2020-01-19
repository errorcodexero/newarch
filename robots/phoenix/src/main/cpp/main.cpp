#include <iostream>
#include "Phoenix.h"

#ifdef SIMULATOR
#endif

#ifdef SIM2
#include <engine/SimulatorEngine.h>
#include <WestCoastDriveFactory.h>

//
// Instantiate a factory for any models we are using
//
xero::models::WestCoastDriveFactory wcfactory;
#endif

int main(int ac, char **av)
{
#ifdef SIM2
    xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
    if (!engine.parseCommandLineArgs(ac, av))
        return 1;
#endif

#ifdef SIMULATOR
    frc::StartRobot<xero::phoenix::Phoenix>(ac, av) ;
#else
    frc::StartRobot<xero::phoenix::Phoenix>() ;
#endif
    return 0 ;
}
