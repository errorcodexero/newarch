#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class LifterHeightMode : public PhaserAutoModeBase
        {
        public:
            LifterHeightMode(xero::base::Robot &robot, double height, bool rel=true) ;
            virtual ~LifterHeightMode() ;
        } ;
    }
}