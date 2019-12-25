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
            /// \param id the message logger id to use for messages from this class     
            /// \param angular Whether the encoder is measuring an angle   
            /// \sa xero::base::MotorFactory
            /// \sa xero::base::XeroEncoder
            /// \sa xero::misc::SettingsParser
            MotorEncoderSubsystem(Subsystem *parent, const std::string &name, uint64_t id, bool angular = false );

            bool isAngular() const {
                return angular_ ;
            }
            bool canAcceptAction(xero::base::ActionPtr action) override;
            void computeState() override;
            bool canAcceptDefaultAction(xero::base::ActionPtr action) override;

            double getPosition() { return speedometer_.getDistance(); }

            xero::misc::Speedometer &getSpeedometer() { return speedometer_; }

            void calibrate();

            void reset() override;

            void setSmartDashboardName(const std::string &name) { smartDashboardName_ = name ; }

            virtual void postHWInit() ;

        private:
            std::shared_ptr<XeroEncoder> encoder_;

            xero::misc::Speedometer speedometer_;

            const std::string configName_;
            std::string smartDashboardName_ ;
            uint64_t msg_id_;

            bool angular_ ;
        };
    }
}
