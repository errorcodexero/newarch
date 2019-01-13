#pragma once

#include "LineFollower.h"
#include <Action.h>

namespace xero {
    namespace phaser {
        class LineFollowerAction : public xero::base::Action {
        public:
            LineFollowerAction(LineFollower &follower) : follower_(follower) {
            }

        protected:
            LineFollower &getGrabber() {
                return follower_ ;
            }

            const LineFollower &getGrabber() const {
                return follower_ ;
            }
            
        private:
            LineFollower &follower_ ;
        } ;
    }
}
