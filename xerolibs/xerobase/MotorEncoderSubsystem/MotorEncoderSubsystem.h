#pragma once

#include <frc/Encoder.h>

#include "SingleMotorSubsystem/SingleMotorSubsystem.h" 

namespace xero {
    namespace base {
        class MotorEncoderSubsystem : public SingleMotorSubsystem {
            friend class MotorEncoderGoToAction;
        public:
            /// \brief Create a new subsystem object
            /// \param parent a reference to the parent subsystem
            /// \param name the name of this subsystem, must be unique across all subsystems
            /// \param config the  base name of the parameter file entries that contains the subsystem definitions
            /// \param id the message logger id to use for messages from this class            
            /// \sa xero::base::MotorFactory
            /// \sa xero::misc::SettingsParser
            MotorEncoderSubsystem(Subsystem *parent, const std::string &name, const std::string config, uint64_t id);

            virtual bool canAcceptAction(xero::base::ActionPtr action);
            virtual void computeState();

            double getPosition() { return position_; }

        private:
            std::shared_ptr<frc::Encoder> encoder_;

            double ticksToUnits(int ticks);
            
            // The number of units per encoder tick.
            double unitsPerTick_;

            // The encoder value corresponding to zero units.
            int zeroTicks_;

            int encoderValue_;
            double position_;

            const std::string configName_;
            uint64_t msg_id_;
        };
    }
}