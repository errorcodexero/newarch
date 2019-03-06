#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class HabCargoMode : public PhaserAutoModeBase
        {
        public:
            HabCargoMode(xero::base::Robot &robot, bool left) ;
            virtual ~HabCargoMode() ;
        } ;
    }
}