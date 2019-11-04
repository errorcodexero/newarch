#include "RanseurAutoModeBase.h"
#include "Ranseur.h"
#include <actions/TerminateAction.h>
#include <actions/ParallelAction.h>
#include <actions/DelayAction.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        RanseurAutoModeBase::RanseurAutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        RanseurAutoModeBase::~RanseurAutoModeBase() 
        {            
        }


    }
}