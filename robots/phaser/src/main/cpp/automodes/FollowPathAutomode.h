#include "automodes/PhaserAutoModeBase.h"

namespace xero {
    namespace phaser {
        class FollowPathAutomode : public PhaserAutoModeBase
        {
        public:
            FollowPathAutomode(xero::base::Robot &robot, const std::string &path, bool reverse = false) ;
            virtual ~FollowPathAutomode() ;
        } ;
    }
}