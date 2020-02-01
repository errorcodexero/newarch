#include "Turret.h"
#include "droidids.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Turret::Turret(Subsystem *parent): MotorEncoderSubsystem(parent, "turret", MSG_GROUP_TURRET, true) 
        {
            setMotor(0.0) ;

            auto act = std::make_shared<xero::base::SingleMotorPowerAction>(*this, 0.0) ;
            setDefaultAction(act) ;            
        }
    }
}