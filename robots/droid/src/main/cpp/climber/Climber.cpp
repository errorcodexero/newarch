
#include "droidids.h"
#include "Climber.h"
#include "Lifter.h"
#include "ClimberAction.h"

#include <frc/DriverStation.h>

using namespace xero::misc;
using namespace xero::base;
using namespace frc;

namespace xero {
    namespace droid {
        Climber::Climber(Subsystem *parent): Subsystem(parent, "climber") {
            msg_id_ = MSG_GROUP_CLIMBER;

            max_height_ = parent->getRobot().getSettingsParser().getDouble("climber:max_height") ;

            lifter_ = std::make_shared<Lifter>(this, "climber:lifter", MSG_GROUP_CLIMBER) ; 
            lifter_->getMotorController()->setCurrentLimit(40) ;
            lifter_->getMotorController()->setNeutralMode(MotorController::NeutralMode::Brake) ;
            lifter_->getMotorController()->resetEncoder() ;
            lifter_->setSmartDashboardName("lifter", false) ;
            addChild(lifter_);

            int travid = parent->getRobot().getSettingsParser().getInteger("hw:climber:traverser:pwmid") ;
            traverser_ = std::make_shared<rev::SparkMax>(travid) ;
        }

        void Climber::run()
        {
            Subsystem::run() ;
        }
        
        void Climber::init(xero::base::LoopType ltype) {
            auto &settings = getRobot().getSettingsParser();
            auto param = "climber:force_field_mode";

            field_mode_ = frc::DriverStation::GetInstance().IsFMSAttached() ||
                (settings.isDefined(param) && settings.getBoolean(param));
        }

        bool Climber::canAcceptAction(ActionPtr action) {
            return std::dynamic_pointer_cast<ClimberAction>(action) != nullptr;
        }
    }
}
