#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class LeftHabLeftCargoMode : public PhaserAutoModeBase
        {
        public:
            LeftHabLeftCargoMode(xero::base::Robot &robot) ;
            virtual ~LeftHabLeftCargoMode() ;
        } ;
    }
}