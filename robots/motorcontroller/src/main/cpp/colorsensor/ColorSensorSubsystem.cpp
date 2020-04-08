#include "ColorSensorSubsystem.h"
#include "motorcontrollerids.h"
#include <Robot.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero
{
    namespace motorcontroller
    {
        ColorSensorSubsystem::ColorSensorSubsystem(Subsystem *parent) : Subsystem(parent, "ColorSensor")
        {
            sensor_ = std::make_shared<rev::ColorSensorV3>(frc::I2C::kOnboard) ;
        }

        ColorSensorSubsystem::~ColorSensorSubsystem()
        {

        }

        void ColorSensorSubsystem::computeMyState()
        {
            auto &logger = getRobot().getMessageLogger() ;
            color_ = sensor_->GetColor() ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_COLORSENSOR) ;
            logger << "Color Sensed:" ;
            logger << " " << color_.red ;
            logger << " " << color_.green ;
            logger << " " << color_.blue ;
            logger.endMessage() ;            
        }
    }
}