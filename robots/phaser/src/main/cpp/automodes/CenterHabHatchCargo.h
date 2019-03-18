#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class CenterHabHatchCargo : public PhaserAutoModeBase
        {
        public:
            CenterHabHatchCargo(xero::base::Robot &robot, bool left, const char *name, const char *desc) ;
            virtual ~CenterHabHatchCargo() ;
        } ;
    }
}