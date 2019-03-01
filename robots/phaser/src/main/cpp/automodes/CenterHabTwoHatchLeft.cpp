#include "automodes/CenterHabTwoHatchLeft.h"
#include "Phaser.h"

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CenterHabTwoHatchLeft::CenterHabTwoHatchLeft(Robot &robot) : PhaserAutoModeBase(robot, "CenterHabTwoHatchLeft", "Start in center, score two on cargo ship, getting extra hatch from left loading station")
        {            
            // auto &phaser = dynamic_cast<Phaser &>(getRobot()) ;
        }

        CenterHabTwoHatchLeft::~CenterHabTwoHatchLeft()
        {            
        }    
    }
}