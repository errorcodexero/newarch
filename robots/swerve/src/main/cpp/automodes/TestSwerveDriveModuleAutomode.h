#include <AutoMode.h>

namespace xero {
    namespace swerve {
        class TestSwerveDriveModuleAutomode : public xero::base::AutoMode
        {
        public:
            TestSwerveDriveModuleAutomode(xero::base::Robot &robot) ;
            virtual ~TestSwerveDriveModuleAutomode() ;
        } ;
    }
}