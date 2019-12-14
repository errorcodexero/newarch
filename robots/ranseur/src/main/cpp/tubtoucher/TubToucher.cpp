#include "TubToucher.h"
#include "TubToucherAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace ranseur {
        TubToucher::TubToucher(Subsystem *parent) : xero::base::Subsystem(parent, "tubtoucher") {
            auto &robot = getRobot();

            int sol = robot.getSettingsParser().getInteger("hw:tubtoucher:solenoid:1_1") ;
            solenoid1_1_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid1_1_->Set(false) ;

            sol = robot.getSettingsParser().getInteger("hw:tubtoucher:solenoid:1_2") ;
            solenoid1_2_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid1_2_->Set(false) ;            

            sol = robot.getSettingsParser().getInteger("hw:tubtoucher:solenoid:2_1") ;
            solenoid2_1_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid2_1_->Set(false) ;   

            sol = robot.getSettingsParser().getInteger("hw:tubtoucher:solenoid:2_2") ;
            solenoid2_2_ = std::make_shared<frc::Solenoid>(sol) ;
            solenoid2_2_->Set(false) ;                         

            deployed_ = false ;
        }

        TubToucher::~TubToucher() {
        }

        void TubToucher::init(LoopType lt)
        {
            if (lt == LoopType::OperatorControl)
            {
                solenoid1_1_->Set(false);
                solenoid1_2_->Set(true);
                solenoid2_1_->Set(false);
                solenoid2_2_->Set(true);                  
            }
        }

        void TubToucher::computeState() {
        }

        bool TubToucher::canAcceptAction(xero::base::ActionPtr action) {
            std::shared_ptr<TubToucherAction> action_p = std::dynamic_pointer_cast<TubToucherAction>(action) ;
            return action_p != nullptr ;
        }
    }
}
