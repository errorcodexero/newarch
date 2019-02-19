#pragma once

#include "phasercameratracker/PhaserCameraTracker.h"
#include <DetectAutoSequence.h>

namespace xero 
{
    namespace phaser 
    {
        class VisionDetectTakeover : public xero::base::DetectAutoSequence
        {
        public:
            VisionDetectTakeover(xero::base::ActionPtr seq, PhaserCameraTracker &camera) : DetectAutoSequence(seq), camera_(camera) {
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