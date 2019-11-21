#include "AutoModeXero.h"
#include <Ranseur.h>
#include <tankdrive/actions/TankDrivePowerAction.h>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        AutoModeXero::AutoModeXero(Robot &robot) : RanseurAutoModeBase(robot, "Mode 0", "The one and only automode")
        {
            ActionPtr act ;
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto tankdrive = ranseur.getRanseurRobotSubsystem()->getTankDrive() ;

            auto action = std::make_shared<TankDrivePowerAction>(*tankdrive, 0.6, 0.6) ;
            pushSubActionPair(tankdrive, action) ;
        }

        AutoModeXero::~AutoModeXero()
        {            
        }
    }
}
