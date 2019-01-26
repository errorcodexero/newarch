#pragma once

#include "ControllerBase.h"
#include <ActionSequence.h>
#include <DetectAutoSequence.h>
/// \file


namespace xero {
    namespace base {
        /// \brief base class for the operator mode controller
        class TeleopController : public ControllerBase {
        public:
            /// \brief create a new controller
            /// \param robot the robot this controller will control
            TeleopController(Robot &robot) ;

            /// \brief destroy the existing controller
            virtual ~TeleopController() ;

            virtual void run() ;

        private:
            std::shared_ptr<ActionSequence> seq_ ;     
            std::list<std::shared_ptr<DetectAutoSequence>> auto_sequences_ ;
            std::shared_ptr<ActionSequence> running_auto_seq_ ;
        } ;
    }
}