#pragma once

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
        class Grabber : public xero::base::Subsystem {
            friend class GrabberCalibrateAction ;
            friend class GrabberHoldCubeAction ;
            friend class GrabberToAngleAction ;
            friend class GrabberPowerAction ;
            
        public:
            Grabber(xero::base::Robot &robot) ;
            virtual ~Grabber() ;
            
            void createNamedSequences() ;
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;

            double getAngle() const {
                return angle_ ;
            }

            int getEncoderValue() const {
                return encoder_->Get() ;
            }

            void calibrate() ;

        private:
            void setMotorValue(double value) ;

        private:
            //
            // The value of the encoder at zero angle (grabber straight out)
            //
            int encoder_zero_ ;

            //
            // The angle of the grabber tha last time computeState was called.  This is
            // only valid if the grabber is calibrated
            //
            double angle_ ;

            //
            // If true, the grabber has been calibrated
            //
            bool calibrated_ ;

            //
            // The number of degree per encoder tick
            //
            double degrees_per_tick_ ;

            //
            // The min angle for the grabber
            //
            double min_angle_ ;
            double max_angle_ ;

            //
            // The grabber hardware
            //
            std::shared_ptr<frc::VictorSP> motor_ ;
            std::shared_ptr<frc::Encoder> encoder_ ;
        } ;
    }
}
