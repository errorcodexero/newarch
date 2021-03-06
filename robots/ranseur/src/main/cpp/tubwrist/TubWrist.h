#pragma once

#include "Subsystem.h"
#include "DebounceBoolean.h"
#include <frc/DigitalInput.h>
#include <frc/VictorSP.h>
#include <memory>
#include <motors/MotorController.h>
#include <motorencodersubsystem/MotorEncoderSubsystem.h>

namespace xero {
    namespace ranseur {
        class TubWrist ;

        class TubWrist : public xero::base::MotorEncoderSubsystem {
       
            public:
            TubWrist(xero::base::Subsystem* parent) ;
            virtual ~TubWrist() ;
        } ;
    }
}
