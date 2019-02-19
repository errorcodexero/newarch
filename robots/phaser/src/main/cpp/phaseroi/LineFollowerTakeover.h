#pragma once

#include <lightsensor/LightSensorSubsystem.h>
#include <DetectAutoSequence.h>

namespace xero 
{
    namespace phaser 
    {
        class LineFollowerTakeover : public xero::base::DetectAutoSequence
        {
        public:
            LineFollowerTakeover(xero::base::ActionPtr seq, xero::base::LightSensorSubsystem &lights) : DetectAutoSequence(seq), lights_(lights) {
            }

            ~LineFollowerTakeover() {                
            }
            
            virtual bool isTakeoverValid() {
                return   lights_.detectedObject() ;
            }

        private:
            xero::base::LightSensorSubsystem &lights_ ;
        };
    }
}