#include "ClimberUpDownAction.h"


using namespace xero::base ;

namespace xero
{
    namespace droid
    {
        ClimberUpDownAction::ClimberUpDownAction(Climber &subsystem, double lift, double trav) : ClimberAction(subsystem)
        {
            action_ = std::make_shared<SingleMotorPowerAction>(*subsystem.getLifter(), lift) ;
            traverse_ = trav ;
        }

        ClimberUpDownAction::ClimberUpDownAction(Climber &subsystem, const std::string &lift, const std::string &trav) : ClimberAction(subsystem)
        {
            action_ = std::make_shared<SingleMotorPowerAction>(*subsystem.getLifter(), lift) ;
            traverse_ = subsystem.getRobot().getSettingsParser().getDouble(trav) ;
        }

        ClimberUpDownAction::ClimberUpDownAction(Climber &subsystem, const std::string &lift, double trav) : ClimberAction(subsystem)
        {
            action_ = std::make_shared<SingleMotorPowerAction>(*subsystem.getLifter(), lift) ;
            traverse_ = trav;
        }

        ClimberUpDownAction::ClimberUpDownAction(Climber &subsystem, double lift, const std::string &trav) : ClimberAction(subsystem)
        {
            action_ = std::make_shared<SingleMotorPowerAction>(*subsystem.getLifter(), lift) ;
            traverse_ = subsystem.getRobot().getSettingsParser().getDouble(trav) ;
        }                

        ClimberUpDownAction::~ClimberUpDownAction()
        {            
        }

        void ClimberUpDownAction::start()
        {
            getSubsystem().getLifter()->setAction(action_, true) ;
            getSubsystem().setTraverserPower(traverse_) ;
        }

        void ClimberUpDownAction::run()
        {
            auto &climber = getSubsystem() ;
            if (action_->getPower() > 0 && climber.getLifter()->getPosition() > climber.getMaxHeight())
                climber.getLifter()->cancelAction() ;
            else if (action_->getPower() < 0 && climber.getLifter()->getPosition() <= 0.0)
                climber.getLifter()->cancelAction() ;
        }

        std::string ClimberUpDownAction::toString()
        {
            std::string ret = "ClimberUpDownAction " ;
            return ret ;
        }
    }
}