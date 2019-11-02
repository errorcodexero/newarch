#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <frc/DigitalInput.h>
#include <frc/VictorSP.h>
#include <memory>
#include <motors/MotorController.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

/// ** need to derive this from the motor-encoder class.. ** ///

namespace xero {
    namespace ranseur {
        class TubWrist ;

        class TubWrist : public xero::base::MotorEncoderSubsystem {
       
            public:
            TubWrist(xero::base::Subsystem* parent) ;
            virtual ~TubWrist() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            virtual void computeState() ;

        } ;
    }
}
