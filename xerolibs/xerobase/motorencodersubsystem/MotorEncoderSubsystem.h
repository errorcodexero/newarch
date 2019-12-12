#pragma once

#include <Speedometer.h>
#include "singlemotorsubsystem/SingleMotorSubsystem.h"
#include "XeroEncoder.h"

namespace xero {
    namespace base {
        /// A Motor/Encoder Subsystem pairs a SingleMotorSubsystem with a XeroEncoder.
        /// The encoder tracks the position of the motor and can be used to measure
        /// distance and to precisely target a desired position.
        /// \sa xero::base::SingleMotorSubsystem
        /// \sa xero::base::XeroEncoder
        /// \sa xero::base::MotorEncoderGoToAction
        class MotorEncoderSubsystem : public SingleMotorSubsystem {
            friend class MotorEncoderGoToAction;
            friend class MotorEncoderHoldAction;
        public:
            /// \brief Create a new subsystem object
            /// \param parent a reference to the parent subsystem
            /// \param name the name of this subsystem, must be unique across all subsystems
            /// \param config the  base name of the parameter file entries that contains the subsystem definitions
            /// \param id the message logger id to use for messages from this class     
            /// \param angular Whether the encoder is measuring an angle   
            /// \sa xero::base::MotorFactory
            /// \sa xero::base::XeroEncoder
            /// \sa xero::misc::SettingsParser
            MotorEncoderSubsystem(Subsystem *parent, const std::string &name, const std::string config,
                                  uint64_t id, bool angular = false );

            bool canAcceptAction(xero::base::ActionPtr action) override;
            void computeState() override;
            bool canAcceptDefaultAction(xero::base::ActionPtr action) override;

            double getPosition() { return speedometer_.getDistance(); }

            void setSpecialCaseFixMe() {
                encoder_->setSpecialCaseFixMe() ;
            }

            xero::misc::Speedometer &getSpeedometer() { return speedometer_; }
            bool isAngular() { return encoder_->isAngular(); }

            void calibrate();

            void reset() override;

            void setSmartDashboardName(const std::string &name) { smartDashboardName_ = name ; }

            void init(LoopType ltype) override;

        private:
            std::shared_ptr<XeroEncoder> encoder_;

            xero::misc::Speedometer speedometer_;

            const std::string configName_;
            std::string smartDashboardName_ ;
            uint64_t msg_id_;
        };
    }
}
