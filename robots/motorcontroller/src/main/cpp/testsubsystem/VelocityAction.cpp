#include "VelocityAction.h"
#include "TestSubsystem.h"
#include <frc/smartdashboard/SmartDashboard.h>

namespace xero {
    namespace motorcontroller {

        VelocityAction::VelocityAction(TestSubsystem &sub): xero::base::MotorEncoderVelocityAction(sub, 0), sub_(sub) 
        {

        }

        VelocityAction::~VelocityAction()
        {

        }

        void VelocityAction::run()
        {
            setTarget(frc::SmartDashboard::GetNumber("velocity", getTarget()));
            MotorEncoderVelocityAction::run();
        }
    }
}
