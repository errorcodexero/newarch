#include "AutoModeXero.h"
#include <Ranseur.h>
#include <bunnyarm/BunnyArm.h>
#include <bunnyarm/BunnyArmDeployAction.h>
#include <actions/DelayAction.h>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        AutoModeXero::AutoModeXero(Robot &robot) : RanseurAutoModeBase(robot, "Mode 0", "The one and only automode")
        {
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;

            pushAction(std::make_shared<DelayAction>(4.0)) ;

            auto act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
            pushSubActionPair(bunnyarm, act) ;
        }

        AutoModeXero::~AutoModeXero()
        {            
        }
    }
}
