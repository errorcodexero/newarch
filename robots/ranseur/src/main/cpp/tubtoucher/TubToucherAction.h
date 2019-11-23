#pragma once

#include <actions/Action.h>

namespace xero {
    namespace ranseur {
        class TubToucher ;

        class TubToucherAction : public xero::base::Action {
        public:
            TubToucherAction(TubToucher &tubtoucher) : tubToucher_(tubtoucher) {                
            }
            
            TubToucher &getTubToucher() {
                return tubToucher_ ;
            }

        private:
            TubToucher &tubToucher_ ;
        } ;
    }
}
