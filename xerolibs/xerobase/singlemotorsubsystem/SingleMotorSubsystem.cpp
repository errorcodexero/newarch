#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, const std::string &motor, uint64_t mid, bool victor) : Subsystem(robot,name) {
            int m = robot.getSettingsParser().getInteger(motor) ;

            index_ = m ;
            msg_id_ = mid ;

            if (victor)
            {
                motor_ = std::make_shared<ctre::phoenix::motorcontrol::can::VictorSPX>(m) ;
            }
            else
                motor_ = std::make_shared<ctre::phoenix::motorcontrol::can::TalonSRX>(m);

            motor_->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake) ;
            current_power_ = 0.0 ;
        }

        SingleMotorSubsystem::SingleMotorSubsystem(Robot & robot, const std::string &name, int m, uint64_t mid, bool victor) : Subsystem(robot,name) {

            index_ = m ;
            msg_id_ = mid ;
            
            if (victor)
                motor_ = std::make_shared<VictorSPX>(m) ;
            else
                motor_ = std::make_shared<TalonSRX>(m);

            motor_->SetNeutralMode(ctre::phoenix::motorcontrol::NeutralMode::Brake) ;
            motor_->ConfigVoltageCompSaturation(12.0, 10) ;
            motor_->EnableVoltageCompensation(true) ;

            current_power_ = 0.0 ;
        }        

        SingleMotorSubsystem::~SingleMotorSubsystem(){
        }

        bool SingleMotorSubsystem::canAcceptAction(ActionPtr action) {
          
            auto coll = std::dynamic_pointer_cast<SingleMotorSubsystemAction>(action) ;
            return coll != nullptr ;
        }
    }
}