#include "AutoMode.h"

namespace xero {
    namespace base {
        class TankDriveScrubMode : public AutoMode
        {
        public:
            TankDriveScrubMode(xero::base::Robot &robot, double power, double duration, bool highgear = true) ;
            virtual ~TankDriveScrubMode() ;
        } ;
    }
}