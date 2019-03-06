#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class CenterHabTwoHatch : public PhaserAutoModeBase
        {
        public:
            CenterHabTwoHatch(xero::base::Robot &robot, bool left, bool second=true) ;
            virtual ~CenterHabTwoHatch() ;
        } ;
    }
}