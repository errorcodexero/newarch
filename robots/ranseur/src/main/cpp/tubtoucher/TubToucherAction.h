#pragma once

#include "tubtoucher/TubToucher.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace ranseur {
        class TubToucher ;

        class TubToucherAction : public xero::base::GenericAction {
        public:
            TubToucherAction(TubToucher &tubtoucher) : GenericAction(tubtoucher.getRobot().getMessageLogger()), tubToucher_(tubtoucher) {                
            }
            
            TubToucher &getTubToucher() {
                return tubToucher_ ;
            }

        private:
            TubToucher &tubToucher_ ;
        } ;
    }
}
