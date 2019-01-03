#pragma once

#include "SingleLED.h"
#include <Subsystem.h>
#include <DigitalOutput.h>

namespace xero
{
    namespace gopigo
    {
        class LEDSubsystem : public xero::base::Subsystem
        {
            friend class LEDSubsystemOnOffAction;
            friend class LEDSubsystemBlinkAction;

          public:
            enum class LED
            {
                LeftEye = 0,
                RightEye = 1,
                LeftBlinker = 2,
                RightBlinker = 3
            };

          public:
            LEDSubsystem(xero::base::Robot &robot);

            virtual ~LEDSubsystem();

            virtual bool canAcceptAction(xero::base::ActionPtr action);

            static std::string ToString(LED e) {
                std::string ret = "????";

                switch(e) {
                case LED::LeftEye:
                    ret = "LeftEye";
                    break;
                case LED::RightEye:
                    ret = "RightEye";
                    break;
                case LED::LeftBlinker:
                    ret = "LeftBlinker";
                    break;
                case LED::RightBlinker:
                    ret = "RightBlinker";
                    break;
                }

                return ret;
            }

        private:
            typedef std::shared_ptr<SingleLED> TriColorLEDPtr;
            typedef std::shared_ptr<frc::DigitalOutput> SimpleLEDPtr;
            std::vector<TriColorLEDPtr> multi_;
            std::vector<SimpleLEDPtr> singles_;
        };
    } // namespace gopigo
} // namespace xero
