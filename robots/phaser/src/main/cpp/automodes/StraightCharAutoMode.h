#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class StraightCharAutoMode : public PhaserAutoModeBase
        {
        public:
            StraightCharAutoMode(xero::base::Robot &robot, double power, double duration, bool highgear = true) ;
            virtual ~StraightCharAutoMode() ;
        } ;
    }
}