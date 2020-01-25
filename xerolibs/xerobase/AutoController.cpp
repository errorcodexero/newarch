#include "AutoController.h"
#include "oi/OISubsystem.h"

namespace xero {
    namespace base {
        void AutoController::run() 
        {
            if (actionptr_ != nullptr)
            {
                if (!started_)
                {
                    actionptr_->start();
                    started_ = true;
                }
                
                auto oi = getRobot().getOI() ;
                auto gp = oi->getDriverGamepad() ;

                //
                // See if we need to switch to teleop.  This is only valid in games where
                // the player can run (via sandstorm like setup) in automode.
                //
                if (gamepad_interrupt_ && gp->isCancelPressed()) {
                    getRobot().switchToTeleop() ;
                    actionptr_->cancel() ;
                    actionptr_ = nullptr ;
                }

                actionptr_ ->run();
                if (actionptr_->isDone())
                    actionptr_ = nullptr ;
            }
        }            
    }
}