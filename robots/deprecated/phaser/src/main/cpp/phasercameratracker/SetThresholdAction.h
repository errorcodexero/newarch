#pragma once

#include "PhaserCameraTracker.h"
#include <Action.h>

namespace xero {
    namespace phaser {
        class SetThresholdAction : public xero::base::Action
        {
        public:
            SetThresholdAction(PhaserCameraTracker &subsystem, double dist) : camera_(subsystem) {
                dist_ =dist ;
            }

            /// \brief Start the action; called once per action when it starts
            virtual void start() {
                camera_.setVisionDetectThreshold(dist_) ;
            }

            /// \brief Manage the action; called each time through the robot loop
            virtual void run() {                
            }

            /// \brief Cancel the action
            virtual void cancel() {
            }

            /// \brief Return true if the action is complete
            /// \returns True if the action is complete
            virtual bool isDone() {
                return true ;
            }

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() {
                std::string ret("SetThresholdAction") ;
                return ret ;
            }

        private:
            double dist_ ;
            PhaserCameraTracker &camera_ ;
        } ;
    }
}
