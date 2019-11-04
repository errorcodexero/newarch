#include "AutoMode.h"
#include <vector>

namespace xero {
    namespace base {
        class FollowPathAutomode : public AutoMode
        {
        public:
            FollowPathAutomode(xero::base::Robot &robot, const std::string &path, bool reverse = false) ;
            FollowPathAutomode(xero::base::Robot &robot, std::vector<std::string> &paths, std::vector<bool> &reverses) ;            
            virtual ~FollowPathAutomode() ;
        } ;
    }
}