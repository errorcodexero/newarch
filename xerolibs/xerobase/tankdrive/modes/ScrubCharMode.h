#include "AutoMode.h"

namespace xero {
    namespace base {
        class ScrubCharMode : public AutoMode
        {
        public:
            ScrubCharMode(xero::base::Robot &robot, double power, double total, bool highgear = true) ;
            virtual ~ScrubCharMode() ;
        } ;
    }
}