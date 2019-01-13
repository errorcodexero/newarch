#pragma once

namespace xero
{
    namespace base {
        class ISteerDistance {
        public:
            /// \brief return a target distance
            /// \returns the target distance
            virtual double getTargetDistance() = 0 ;
        } ;
    }
}