#include "SingleMotorSubsystem.h"
#include "Robot.h"
#include "motors/MotorController.h"
#include "motors/MotorFactory.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, const std::string &motor, uint64_t mid) : Subsystem(robot,name) {
            msg_id_ = mid ;
            motor_ = robot.getMotorFactory()->createMotor(motor);
            motor_->setNeutralMode(MotorController::NeutralMode::Brake) ;
            current_power_ = 0.0 ;
        }

        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, std::shared_ptr<MotorController> motor, uint64_t mid) : Subsystem(robot,name) {
            msg_id_ = mid ;
            motor_ = motor_;
            motor_->setNeutralMode(MotorController::NeutralMode::Brake);
            current_power_ = 0.0 ;
        } 

        void SingleMotorSubsystem::setMotor(double power)  {
                motor_->set(power) ;
                current_power_ = power ;
            }       

        SingleMotorSubsystem::~SingleMotorSubsystem(){
        }

        bool SingleMotorSubsystem::canAcceptAction(ActionPtr action) {
          
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}