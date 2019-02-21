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
            LineFollowerTakeover(std::shared_ptr<xero::base::TeleopController> teleop, xero::base::ActionPtr seq, xero::base::LightSensorSubsystem &lights) : DetectAutoSequence(teleop, seq), lights_(lights) {
            }

            ~LineFollowerTakeover() {                
            }
            
            virtual bool isTakeoverValid() {
                bool ret = lights_.detectedObject() ;
                if (ret)
                    removeMe() ;

                return ret ;
            }

        private:
            xero::base::LightSensorSubsystem &lights_ ;
        };
    }
}