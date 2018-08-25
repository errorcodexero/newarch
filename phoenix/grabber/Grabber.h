#pragma once

#include <Subsystem.h>
#include <Encoder.h>
#include <VictorSP.h>
#include <cassert>
#include <memory>

namespace xero {
    namespace phoenix {
        class Grabber : public xero::base::Subsystem {
            friend class GrabberCalibrateAction;
            friend class GrabberDutyCycleAction;
            friend class GrabberHoldCubeAction;
            friend class GrabberToAngleAction;
        public:
            Grabber(xero::base::Robot & robot);
            virtual ~Grabber(); 
            virtual void computeState();

            int getEncoderTicks() const {
                return encoder_ticks_;
            }

            double getAngle() const{
                assert(calibrated_);
                return angle_;
            }

		protected:
			/// \brief check that a Action is valid for a subsystem
			/// \param Action the Action to check for a subsystem
			/// \return true if the action is valid for a subsystem
			virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            void calibrate(){
                encoder_->Reset();
                calibrated_ = true;
            }

        private:
            std::shared_ptr<frc::VictorSP> motor_;
            std::shared_ptr<frc::Encoder> encoder_;
            double min_angle_;
            double max_angle_;
            double degrees_per_tick_;
            int encoder_ticks_;
            bool calibrated_;
            double angle_;
        };
    }
}