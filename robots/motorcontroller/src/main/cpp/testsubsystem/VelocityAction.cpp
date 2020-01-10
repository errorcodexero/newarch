#include "VelocityAction.h"
#include "TestSubsystem.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <networktables/NetworkTableValue.h>
#include <wpi/StringMap.h>
#include <cmath>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace motorcontroller {
        static frc::SimpleWidget makeWidget() {
            wpi::StringMap<std::shared_ptr<nt::Value>> propmap;
            propmap.insert(std::make_pair("min", nt::Value::MakeDouble(0.0)));
            propmap.insert(std::make_pair("max", nt::Value::MakeDouble(6000.0)));
            return frc::Shuffleboard::GetTab("SmartDashboard").Add("Velocity", static_cast<double>(0.0)).WithWidget(frc::BuiltInWidgets::kNumberSlider).WithProperties(propmap);
        }

        VelocityAction::VelocityAction(TestSubsystem &sub): xero::base::MotorEncoderVelocityAction(sub, 0), sub_(sub), widget_(makeWidget())
        {
            
        }

        VelocityAction::~VelocityAction()
        {
        }

        void VelocityAction::run()
        {
            double target = widget_.GetEntry().GetDouble(getTarget()) ;
            if (std::fabs(target - getTarget()) > 100)
            {
                auto &logger = getSubsystem().getRobot().getMessageLogger();
                logger.startMessage(MessageLogger::MessageType::debug, getSubsystem().getMsgID());
                logger << "New Target " << target;
                logger.endMessage();
                //setTarget(target);
            }
            MotorEncoderVelocityAction::run();
        }
    }
}
