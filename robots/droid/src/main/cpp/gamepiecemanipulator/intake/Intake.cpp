#include "Intake.h"
#include "droidids.h"
#include "CollectAction.h"
#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <motors/MotorFactory.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Intake::Intake(Subsystem *parent): SingleMotorSubsystem(parent, "intake", MSG_GROUP_INTAKE) 
        {
            lifter_ = std::make_shared<MotorEncoderSubsystem>(this, "intake:lifter", MSG_GROUP_INTAKE) ;
            addChild(lifter_) ;

            auto act = std::make_shared<xero::base::SingleMotorPowerAction>(*lifter_, 0.0) ;
            lifter_->setDefaultAction(act) ;
        }
    }
}
