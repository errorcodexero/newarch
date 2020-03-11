#include "BlinkyAction.h"
#include "BlinkySubsystem.h"
#include <Robot.h>

namespace xero
{
    namespace droid
    {
        BlinkyAction::BlinkyAction(BlinkySubsystem &sub, Pattern pattern) : Action(sub.getRobot().getMessageLogger()), subsystem_(sub)
        {            
            pattern_ = pattern ;
        }

        BlinkyAction::~BlinkyAction()
        {            
        }

        void BlinkyAction::start()
        {
            if (pattern_ == Pattern::RandomPattern)
            {
                uint8_t p = static_cast<uint8_t>(rand() % 4 + 5) ;
                subsystem_.setPattern(p) ;
            }
            else
            {
                subsystem_.setPattern(static_cast<uint8_t>(pattern_)) ;
            }
        }
    }
}