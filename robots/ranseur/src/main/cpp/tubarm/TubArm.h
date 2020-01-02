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
        class TubArm ;

        class TubArm : public xero::base::MotorEncoderSubsystem {
          
        public:
            TubArm(xero::base::Subsystem* parent) ;
            virtual ~TubArm() ;

            virtual void init(xero::base::LoopType lt) ;
            virtual void postHWInit() ;
        } ;
    }
}
