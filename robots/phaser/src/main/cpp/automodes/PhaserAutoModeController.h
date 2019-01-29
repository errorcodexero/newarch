#pragma once

#include <AutoController.h>
#include <ActionSequence.h>
#include <string>

namespace xero {
    namespace phaser {
        class PhaserAutoModeController : public xero::base::AutoController {
        public:
            PhaserAutoModeController(xero::base::Robot &robot) ;
            virtual ~PhaserAutoModeController() ;

            virtual void updateAutoMode(int sel, const std::string &gamedata) ;

        private:
            xero::base::ActionSequencePtr createStraightCharAutoMode() ;
            xero::base::ActionSequencePtr createScrubCharAutoMode() ;
            xero::base::ActionSequencePtr createTestOne() ;
            xero::base::ActionSequencePtr createTestTwo() ;
            xero::base::ActionSequencePtr createFollowLine() ;
            xero::base::ActionSequencePtr testHatchHolder() ;
        } ;
    }
}
