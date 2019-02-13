#include "AutoMode.h"
#include "basegroups.h"
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc ;

namespace xero
{
    namespace base
    {

        AutoMode::AutoMode(xero::misc::MessageLogger &logger, const std::string &name, const std::string &desc) : ActionSequence(logger) , name_(name), desc_(desc)
        {
            
        }

        AutoMode::AutoMode(xero::misc::MessageLogger &logger, const std::string &name) : ActionSequence(logger) , name_(name)
        {
        }

        std::string AutoMode::toString()
        {
            std::string result ;
            result += name_;
            result += ActionSequence::toString() ;
            return result ;
        }

    }
}