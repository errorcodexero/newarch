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

            /// \brief run the TeleopController, called once each time through the robot loop
            virtual void run() ;

            /// \brief add a takeover detector to the teleop controller
            /// This method adds a teleop takeover object to the telop controller.  If the 
            /// detector object returns true from isTakeoverValid(), then the teleop controller
            /// yields control to the action stor4ed in the DetectAutoSequence class.
            /// \param detector the detector to add to the teleop controller
            void addDetector(std::shared_ptr<DetectAutoSequence> detector) {
                auto_sequences_.push_back(detector) ;
            }

            /// \brief remove all detectors from the teleop controller
            void clearDetectors() {
                auto_sequences_.clear() ;
            }

            /// \brief remove a specific detector from the telop controller
            /// \param det the detector to remove
            void removeDetector(std::shared_ptr<DetectAutoSequence> det) ;

            /// \brief print the detectors attached to the telop controller
            /// The detectors are printed using the message logger
            void printDetectors() ;

        private:
            std::shared_ptr<AutoMode> seq_ ;     
            std::list<std::shared_ptr<DetectAutoSequence>> auto_sequences_ ;
            std::shared_ptr<DetectAutoSequence> running_detector_ ;
        } ;
    }
}