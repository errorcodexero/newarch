#pragma once

namespace xero
{
    namespace base {
        class ISteerAngle {
        public:
            /// \brief return a target angle to steer to.
            /// \returns the target angle
            virtual double getTargetAngle() = 0 ;
        } ;
    }
}