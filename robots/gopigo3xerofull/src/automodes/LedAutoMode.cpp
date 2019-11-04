#include "LedAutoMode.h"
#include "GoPiGo3Xero.h"
#include "gopigo3sub/GoPiGo3Subsystem.h"
#include "ledsubsystem/LEDSubsystem.h"
#include "ledsubsystem/LEDSubsystemOnOffAction.h"

using namespace xero::base ;

namespace xero {
    namespace gopigo3 {
        LedAutoMode::LedAutoMode(xero::base::Robot &robot) : AutoMode(robot, "LedAutoMode", "Blink the four leds on the gopigo3 robot")
        {
            ActionPtr action;
            GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(robot);
            auto sub = xerorobot.getGoPiGoSubsystem();
            auto leds = sub->getLEDSubsystem();

            action = std::make_shared<LEDSubsystemOnOffAction>(*leds, LEDSubsystem::LED::LeftEye, 0.0, 1.0, 0.0);
            pushSubActionPair(leds, action);

            action = std::make_shared<LEDSubsystemOnOffAction>(*leds, LEDSubsystem::LED::RightEye, 0.0, 0.0, 0.0);
            pushSubActionPair(leds, action);

            action = std::make_shared<LEDSubsystemOnOffAction>(*leds, LEDSubsystem::LED::LeftBlinker, 0.0, 0.0, 0.0);
            pushSubActionPair(leds, action);

            action = std::make_shared<LEDSubsystemOnOffAction>(*leds, LEDSubsystem::LED::RightBlinker, 0.0, 0.0, 0.0);
            pushSubActionPair(leds, action);
        }

        LedAutoMode::~LedAutoMode()
        {            
        }
    }
}