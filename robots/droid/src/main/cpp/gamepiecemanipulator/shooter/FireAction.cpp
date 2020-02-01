#include "FireAction.h"
#include "Shooter.h"
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <networktables/NetworkTableValue.h>
#include <wpi/StringMap.h>
#include <cmath>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
      
        FireAction::FireAction(Shooter &sub): xero::base::MotorEncoderVelocityAction(sub, 0), sub_(sub)
        {
            
        }

        FireAction::~FireAction()
        {
        }

        void FireAction::run()
        {
            double target = 0 ;                                  //TODO: calculate target velocity
            setTarget(target);
            frc::SmartDashboard::PutNumber("tvel", getTarget());
            MotorEncoderVelocityAction::run();
        }
    }
}
