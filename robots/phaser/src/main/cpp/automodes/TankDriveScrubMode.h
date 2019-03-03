#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class TankDriveScrubMode : public PhaserAutoModeBase
        {
        public:
            TankDriveScrubMode(xero::base::Robot &robot, double power, double duration, bool highgear = true) ;
            virtual ~TankDriveScrubMode() ;
        } ;
    }
}