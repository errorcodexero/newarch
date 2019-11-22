#include "AutoMode.h"

namespace xero {
    namespace base {
        class StraightCharAutoMode : public AutoMode
        {
        public:
            StraightCharAutoMode(xero::base::Robot &robot, double power, double duration, bool highgear = true) ;
            
            virtual ~StraightCharAutoMode() ;
        } ;
    }
}