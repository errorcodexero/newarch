
#include "droidids.h"
#include "Climber.h"

using namespace xero::misc;
using namespace xero::base;
using namespace frc;

namespace xero {
    namespace droid {
        Climber::Climber(Subsystem *parent): Subsystem(parent, "climber") {
            msg_id_ = MSG_GROUP_CLIMBER;

            lifter_ = std::make_shared<MotorEncoderSubsystem>(this, "climber:lifter", MSG_GROUP_CLIMBER) ; 
            lifter_->getMotorController()->setCurrentLimit(40) ;
            lifter_->getMotorController()->setNeutralMode(MotorController::NeutralMode::Brake) ;
            lifter_->setSmartDashboardName("lifter") ;

            //traverser_ = std::make_shared<MotorEncoderSubsystem>(this, "climber:traverser", MSG_GROUP_CLIMBER) ;
            //traverser_->getMotorController()->setCurrentLimit(10) ;

            // for servos put param value in "this"// 
            //TODO change param in .dat file & pull the value into this file : pwmid
            int servo_r_id_ = getRobot().getSettingsParser().getInteger("hw:climber:servo:r:pwmid") ;
            servo_r_ = std::make_shared<Servo>(servo_r_id_) ;

            int servo_l_id_ = getRobot().getSettingsParser().getInteger("hw:climber:servo:l:pwmid") ;
            servo_l_ = std::make_shared<Servo>(servo_l_id_) ;

            /// instantiate the 2 servos
            /// read values from params/config file for servos and assign to the servos... 
            /// example for this is gamepiecemanipulator    

        }
    }
}
