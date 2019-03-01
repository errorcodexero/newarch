#include "PhaserAutoModeBase.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        PhaserAutoModeBase::PhaserAutoModeBase(Robot &robot, const std::string &name, const std::string &desc) : AutoMode(robot, name, desc)
        {            
        }

        PhaserAutoModeBase::~PhaserAutoModeBase() 
        {            
        }
    }
}