#include "SingleMotorPowerAutoMode.h"
#include "SingleMotorPowerAction.h"

namespace xero
{
    namespace base
    {
        SinglePowerAutoMode::SinglePowerAutoMode(Robot &robot, std::shared_ptr<SingleMotorSubsystem> ptr, const std::string &name, 
                        const std::string &desc, double power) : AutoMode(robot, name, desc)
        {
            auto act = std::make_shared<SingleMotorPowerAction>(*ptr.get(), power) ;
            pushSubActionPair(ptr, act) ;
        }

        SinglePowerAutoMode::SinglePowerAutoMode(Robot &robot,std::shared_ptr<SingleMotorSubsystem> ptr, const std::string &name, 
                        const std::string &desc, double power, double duration) : AutoMode(robot, name, desc)
        {
            auto act = std::make_shared<SingleMotorPowerAction>(*ptr.get(), power, duration) ;
            pushSubActionPair(ptr, act) ;
        }

        SinglePowerAutoMode::~SinglePowerAutoMode()
        {

        }
    }
}