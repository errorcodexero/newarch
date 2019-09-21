#pragma once

/// \file

namespace xero {
    namespace base {
        class MotorController {
        public:
            enum class NeutralMode { Coast, Brake };

            /// \brief Create a new motor object
            MotorController() {}

            /// \brief Sets the motor's power.
            /// \param percent The power to set, in the range [-1, 1].
            virtual void set(double percent) = 0;

            /// \brief Sets whether the motor is inverted.
            /// \param inverted Whether the motor should be inverted.
            virtual void setInverted(bool inverted) = 0;

            /// \brief Sets whether the motor will brake or coast.
            /// \param neutralMode The desired neutral mode.
            virtual void setNeutralMode(NeutralMode neutralMode) = 0;
        };
}
}
