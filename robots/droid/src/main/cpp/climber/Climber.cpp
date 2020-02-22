
#include "droidids.h"
#include "Climber.h"

using namespace xero::misc;
using namespace xero::base;
using namespace frc;

namespace xero {
    namespace droid {
        Climber::Climber(Subsystem *parent): Subsystem(parent, "climber") {
            msg_id_ = MSG_GROUP_CLIMBER;

            max_height_ = parent->getRobot().getSettingsParser().getDouble("climber:max_height") ;

            lifter_ = std::make_shared<MotorEncoderSubsystem>(this, "climber:lifter", MSG_GROUP_CLIMBER) ; 
            lifter_->getMotorController()->setCurrentLimit(40) ;
            lifter_->getMotorController()->setNeutralMode(MotorController::NeutralMode::Brake) ;
            lifter_->getMotorController()->resetEncoder() ;            
            lifter_->setSmartDashboardName("lifter") ;

            int travid = parent->getRobot().getSettingsParser().getInteger("hw:climber:traverser:pwmid") ;
            traverser_ = std::make_shared<frc::PWMSparkMax>(travid) ;
        }
    }
}
