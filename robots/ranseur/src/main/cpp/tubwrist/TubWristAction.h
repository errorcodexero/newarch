#pragma once

#include <actions/Action.h>

namespace xero {
    namespace ranseur {
        class TubWrist ;

        class TubWristAction : public xero::base::Action {
        public:
            TubWristAction(TubWrist &tubwrist) : tubwrist_(tubwrist) {
            }

            virtual ~TubWristAction() {
            }

            TubWrist &getTubWrist() {
                return tubwrist_ ;
            }

        private:
            TubWrist &tubwrist_ ;
        } ;
    }
}
