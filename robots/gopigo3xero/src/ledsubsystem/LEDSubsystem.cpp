#include "LEDSubsystem.h"
#include "LEDSubsystemAction.h"

using namespace xero::base;
using namespace frc;

namespace xero
{
    namespace gopigo3 {
        LEDSubsystem::LEDSubsystem(xero::base::Robot &robot):Subsystem(robot, "LEDSubsystem")
        {
            multi_.push_back(std::make_shared<SingleLED>(0, 1, 2));
            multi_.push_back(std::make_shared<SingleLED>(3, 4, 5));

            singles_.push_back(std::make_shared<DigitalOutput>(0));
            singles_.push_back(std::make_shared<DigitalOutput>(1));
        }

        LEDSubsystem::~LEDSubsystem()
        {
        }

        bool LEDSubsystem::canAcceptAction(ActionPtr action)
        {
            auto act = std::dynamic_pointer_cast<LEDSubsystemAction>(action);
            return act != nullptr;
        }
    }
}