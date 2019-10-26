#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <frc/DigitalInput.h>
#include <frc/VictorSP.h>
#include <memory>
#include <motors/MotorController.h>

namespace xero {
    namespace ranseur {
        class TubWrist ;

        class TubWrist : public xero::base::Subsystem {
          
            public:
            TubWrist(xero::base::Subsystem* parent) ;
            virtual ~TubWrist() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

            void createNamedSequences() {
            }

            std::shared_ptr<xero::base::MotorController> tubwrist_ ;

        } ;
    }
}
