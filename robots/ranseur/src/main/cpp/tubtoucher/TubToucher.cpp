#include "TubToucher.h"
#include "TubToucherAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace ranseur {
        TubToucher::TubToucher(Subsystem *parent) : xero::base::Subsystem(parent, "tubtoucher") {
            auto &robot = getRobot();

            int sol = robot.getSettingsParser().getInteger("hw:tubtoucher:solenoid") ;
            solenoid_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid_->Set(false) ;

            deployed_ = false ;
        }

        TubToucher::~TubToucher() {
        }

        void TubToucher::computeState() {
            if (solenoid_->Get())
                deployed_ = true ;
            else
                deployed_ = false ;
        }

        bool TubToucher::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<TubToucherAction> action_p = std::dynamic_pointer_cast<TubToucherAction>(action) ;
            return action_p != nullptr ;
        }
    }
}
