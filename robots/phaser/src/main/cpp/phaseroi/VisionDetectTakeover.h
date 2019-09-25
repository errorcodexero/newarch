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
                std::cout << "Created Vision Takeover object" << std::endl ;
            }

            ~VisionDetectTakeover() {                
            }

            virtual bool isTakeoverValid() {
                if (camera_.shouldTerminate()) {
                    std::cout << "Detected vision target" << std::endl ;
                    return true ;
                }

                return false ;
            }

        private:
            PhaserCameraTracker &camera_ ;
        };
    }
}