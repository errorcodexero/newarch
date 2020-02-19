#include "ClimberTraverseAction.h"

namespace xero
{
    namespace droid
    {
        ClimberTraverseAction::ClimberTraverseAction(Climber &subsystem, double p) : ClimberAction(subsystem)
        {
            power_ = p ;
        }

        ClimberTraverseAction::ClimberTraverseAction(Climber &subsystem, const std::string &name) : ClimberAction(subsystem)
        {
            power_ = subsystem.getRobot().getSettingsParser().getDouble(name) ;
        }

        ClimberTraverseAction::~ClimberTraverseAction()
        {
            
        }

        void ClimberTraverseAction::start()
        {
            getSubsystem().setTraverserPower(power_) ;
        }

        void ClimberTraverseAction::run()
        {
        }

        std::string ClimberTraverseAction::toString()
        {
            std::string ret = "ClimberTraverseAction " + std::to_string(power_) ;
            return ret ;
        }
    }
}