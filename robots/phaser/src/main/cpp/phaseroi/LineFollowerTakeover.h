#pragma once

#include <lightsensor/LightSensorSubsystem.h>
#include <DetectAutoSequence.h>
#include <TeleopController.h>

namespace xero 
{
    namespace phaser 
    {
        class LineFollowerTakeover : public xero::base::DetectAutoSequence
        {
        public:
            LineFollowerTakeover(std::shared_ptr<xero::base::TeleopController> teleop, xero::base::ActionPtr seq, xero::base::LightSensorSubsystem &lights) 
                        : DetectAutoSequence("linefollower", teleop, seq), lights_(lights) {
            }

            ~LineFollowerTakeover() {                
            }
            
            virtual bool isTakeoverValid() {
                return lights_.detectedObject() ;
            }

        private:
            xero::base::LightSensorSubsystem &lights_ ;
        };
    }
}