#include "AutoModeXero.h"
#include <Ranseur.h>
#include <bunnyarm/BunnyArm.h>
#include <bunnyarm/BunnyArmDeployAction.h>
#include <tubcollector/TubCollector.h>
#include <tubcollector/TubCollectorTubAction.h>
#include <tubcollector/TubCollectorEjectTubAction.h>
#include <actions/DelayAction.h>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        AutoModeXero::AutoModeXero(Robot &robot) : RanseurAutoModeBase(robot, "Mode 0", "The one and only automode")
        {
            ActionPtr act ;
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto bunnyarm = ranseur.getRanseurRobotSubsystem()->getBunnyArm() ;
            auto tubcollector = ranseur.getRanseurRobotSubsystem()->getTubManipulatorSubsystem()->getTubCollector() ;

            pushAction(std::make_shared<DelayAction>(2.0)) ;

            act = std::make_shared<BunnyArmDeployAction>(*bunnyarm, true) ;
            pushSubActionPair(bunnyarm, act) ;

            pushAction(std::make_shared<DelayAction>(2.0)) ;

            act = std::make_shared<TubCollectorTubAction>(*tubcollector) ;
            pushSubActionPair(tubcollector, act) ;

            pushAction(std::make_shared<DelayAction>(10.0)) ;

            act = std::make_shared<TubCollectorEjectTubAction>(*tubcollector, "tubcollector:eject:power", "tubcollector:eject:duration") ;
            pushSubActionPair(tubcollector, act) ;
        }

        AutoModeXero::~AutoModeXero()
        {            
        }
    }
}
