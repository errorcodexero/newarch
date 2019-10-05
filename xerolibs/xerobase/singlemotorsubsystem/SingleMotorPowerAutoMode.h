#include "AutoMode.h"
#include "SingleMotorSubsystem.h"

namespace xero
{
    namespace base 
    {
        class SinglePowerAutoMode : public AutoMode
        {
        public:
            SinglePowerAutoMode(Robot &robot, std::shared_ptr<SingleMotorSubsystem> ptr, const std::string &name, 
                                const std::string &desc, double power) ;
            SinglePowerAutoMode(Robot &robot, std::shared_ptr<SingleMotorSubsystem> ptr, const std::string &name, 
                                const std::string &desc, double power, double duration) ;            
            virtual ~SinglePowerAutoMode() ;

        private:
        } ;
    }
}
