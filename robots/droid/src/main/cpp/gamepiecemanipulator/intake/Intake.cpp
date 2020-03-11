#include "Intake.h"
#include "IntakeAction.h"
#include "droidids.h"
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Intake::Intake(Subsystem *parent): MotorEncoderSubsystem(parent, "intake", MSG_GROUP_INTAKE) {

            std::string collectparam = "intake:collect" ;
            std::string motorparam = HWPrefix + collectparam + ":motor" ;
            collector_ = getRobot().getMotorFactory()->createMotor(motorparam) ;
            setSmartDashboardName("intake") ;
        }

        Intake::~Intake()
        {
        }

        bool Intake::canAcceptAction(ActionPtr act)
        {
            auto inact = std::dynamic_pointer_cast<IntakeAction>(act) ;
            return inact != nullptr ;
        }
    }
}
