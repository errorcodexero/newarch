#include <iostream>
#include "Droid.h"

#ifdef SIM2
#include "DroidOIModel.h"
#include "IntakeModel.h"
#include "ConveyorModel.h"
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
USE_SIMULATION_MODEL(DroidOIModel, "droidoi") ;
USE_SIMULATION_MODEL(IntakeModel, "intake") ;
USE_SIMULATION_MODEL(ConveyorModel, "conveyor") ;
USE_SIMULATION_MODEL(LimeLightModel, "limelight") ;
#endif

int main(int ac, char **av)
{
#ifdef SIM2
    xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
    if (engine.start(ac, av) != xero::sim2::SimulatorEngine::ErrorCode::NoError)
        return 1;
#endif

    frc::StartRobot<xero::droid::Droid>() ;

    return 0 ;
}
