#pragma once

#include "ControllerBase.h"
#include <AutoMode.h>
#include <DetectAutoSequence.h>
#include <algorithm>

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

            void addDetector(std::shared_ptr<DetectAutoSequence> det) {
                auto_sequences_.push_back(det) ;
            }

            void clearDetectors() {
                auto_sequences_.clear() ;
            }

            void removeDetector(DetectAutoSequence *det) ;

        private:
            std::shared_ptr<AutoMode> seq_ ;     
            std::list<std::shared_ptr<DetectAutoSequence>> auto_sequences_ ;
            std::shared_ptr<Action> running_auto_seq_ ;
        } ;
    }
}