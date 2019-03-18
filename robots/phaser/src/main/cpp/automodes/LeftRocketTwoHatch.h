#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class LeftRocketTwoHatch : public PhaserAutoModeBase
        {
        public:
            LeftRocketTwoHatch(xero::base::Robot &robot, bool second=true) ;
            virtual ~LeftRocketTwoHatch() ;
        } ;
    }
}