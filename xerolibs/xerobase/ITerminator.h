#pragma once

namespace xero
{
    namespace base {
        class ITerminator {
        public:
            /// \brief returns true if the current action should be terminated
            /// \returns the termination state of the current action
            virtual bool shouldTerminate() = 0 ;
        } ;
    }
}