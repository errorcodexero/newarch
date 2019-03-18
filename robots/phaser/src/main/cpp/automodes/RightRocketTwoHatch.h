#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class RightRocketTwoHatch : public PhaserAutoModeBase
        {
        public:
            RightRocketTwoHatch(xero::base::Robot &robot, bool second=true) ;
            virtual ~RightRocketTwoHatch() ;
        } ;
    }
}