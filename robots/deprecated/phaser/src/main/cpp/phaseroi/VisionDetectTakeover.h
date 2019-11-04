#pragma once

#include "phasercameratracker/PhaserCameraTracker.h"
#include <DetectAutoSequence.h>
#include <TeleopController.h>

namespace xero 
{
    namespace phaser 
    {
        class VisionDetectTakeover : public xero::base::DetectAutoSequence
        {
        public:
            VisionDetectTakeover(std::shared_ptr<xero::base::TeleopController> teleop, xero::base::ActionPtr seq, PhaserCameraTracker &camera) 
                    : DetectAutoSequence("vision", teleop, seq), camera_(camera) {
            }

            ~VisionDetectTakeover() {                
            }

            virtual bool isTakeoverValid() {
                return camera_.shouldTerminate() ;
            }

        private:
            PhaserCameraTracker &camera_ ;
        };
    }
}