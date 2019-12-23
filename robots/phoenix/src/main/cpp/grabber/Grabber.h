#pragma once

#include "motorencodersubsystem/MotorEncoderSubsystem.h"
#include <Subsystem.h>
#include <frc/VictorSP.h>
#include <frc/Encoder.h>
#include <PIDCtrl.h>
#include <limits>
#include <vector>

namespace xero {
    namespace phoenix {
        /// \brief the grabber subsystem that opens and closes the intake arms.
        /// This subsystem controls a single motor that controls the angle of the
        /// grabber arms.  There is an encoder on the motor that is used to detect
        /// athe angle of the grabber arms.
        class Grabber : public xero::base::MotorEncoderSubsystem {
            friend class GrabberAction ;
            
        public:
            Grabber(xero::base::Subsystem *parent) ;
            virtual ~Grabber() ;
            
        private:
            //
            // The min angle for the grabber
            //
            double min_angle_ ;
            double max_angle_ ;
        } ;
    }
}
