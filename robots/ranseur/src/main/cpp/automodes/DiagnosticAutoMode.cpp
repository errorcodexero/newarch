#include "DiagnosticAutoMode.h"
#include "ranseurrobotsubsystem/RanseurDiagnosticAction.h"
#include <Ranseur.h>
#include <memory>

using namespace xero::base ;

namespace xero
{
    namespace ranseur 
    {
        DiagnosticAutoMode::DiagnosticAutoMode(Robot &robot) : RanseurAutoModeBase(robot, "Mode 0", "The one and only automode")
        {
            auto &ranseur = dynamic_cast<Ranseur &>(getRobot()) ;
            auto robsub = ranseur.getRanseurRobotSubsystem() ;
            auto act = std::make_shared<RanseurDiagnosticAction>(*robsub) ;
            pushSubActionPair(robsub, act) ;
        }

        DiagnosticAutoMode::~DiagnosticAutoMode()
        {            
        }
    }
}

