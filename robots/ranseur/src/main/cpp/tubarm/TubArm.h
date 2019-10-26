#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <frc/DigitalInput.h>
#include <frc/VictorSP.h>
#include <memory>
#include <motors/MotorController.h>

namespace xero {
    namespace ranseur {
        class TubArm ;

        class TubArm : public xero::base::Subsystem {
          
            public:
            TubArm(xero::base::Subsystem* parent) ;
            virtual ~TubArm() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

            void createNamedSequences() {
            }

            std::shared_ptr<xero::base::MotorController> tubarm_ ;

        } ;
    }
}
