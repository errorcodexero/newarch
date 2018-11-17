#include "SingleMotorVoltageAction.h"
#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::base;

namespace xero {
    namespace base {
            SingleMotorVoltageAction::SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, double v) : SingleMotorSubsystemAction(single_motor_subsystem) {
                duty_cycle_ = v ;
            }

            SingleMotorVoltageAction::SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name) : SingleMotorSubsystemAction(single_motor_subsystem) {
                duty_cycle_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(name) ;
            }

            SingleMotorVoltageAction::~SingleMotorVoltageAction() {

            }
            void SingleMotorVoltageAction::start() {
                getSubsystem().getMotor()->Set(duty_cycle_) ;
            }

            void SingleMotorVoltageAction::run() {
            }

            bool SingleMotorVoltageAction::isDone() {
                return true ;
            }

            void SingleMotorVoltageAction::cancel() {
            }

            std::string SingleMotorVoltageAction::toString() {
                std::string result = "SingleMotorVoltageAction " ;
                result += std::to_string(duty_cycle_) ;
                return result ;
            }
    }
}
