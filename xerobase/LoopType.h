#pragma once

namespace xero {
    namespace base {
        /// \brief The type of robot loop we are running
        enum class LoopType : int {
            OperatorControl = 0,                ///< Operator control robot loop
            Autonomous = 1,                     ///< Autnomous robot loop
            Test = 2,                           ///< Test robot loop
            MaxValue = 3,                       ///< Total number of loop types
        } ;     
    }
}