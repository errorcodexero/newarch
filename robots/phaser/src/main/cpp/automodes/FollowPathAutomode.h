#include "automodes/PhaserAutoModeBase.h"
#include <vector>

namespace xero {
    namespace phaser {
        class FollowPathAutomode : public PhaserAutoModeBase
        {
        public:
            FollowPathAutomode(xero::base::Robot &robot, const std::string &path, bool reverse = false) ;
            FollowPathAutomode(xero::base::Robot &robot, std::vector<std::string> &paths, std::vector<bool> &reverses) ;            
            virtual ~FollowPathAutomode() ;
        } ;
    }
}