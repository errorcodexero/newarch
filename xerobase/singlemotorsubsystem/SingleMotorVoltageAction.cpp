#include "SingleMotorVoltageAction.h"
#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::base;

namespace xero {
    namespace base {
            SingleMotorVoltageAction::SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, double v) : SingleMotorSubsystemAction(single_motor_subsystem) {
                duty_cycle_ = v ;
				timed_ = false ;
            }

            SingleMotorVoltageAction::SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name) : SingleMotorSubsystemAction(single_motor_subsystem) {
                duty_cycle_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(name) ;
				timed_ = false ;
            }

            SingleMotorVoltageAction::SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, double v, double d) : SingleMotorSubsystemAction(single_motor_subsystem) {
                duty_cycle_ = v ;
				timed_ = true ;
				duration_ = d ;
            }

            SingleMotorVoltageAction::SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name, 
															const std::string &durname) : SingleMotorSubsystemAction(single_motor_subsystem) {
                duty_cycle_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(name) ;
				timed_ = true ;
				duration_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(durname) ;
            }			

            SingleMotorVoltageAction::~SingleMotorVoltageAction() {
            }

            void SingleMotorVoltageAction::start() {
                getSubsystem().setMotor(duty_cycle_) ;
				if (timed_) {
					is_done_ = false ;
					start_ = getSubsystem().getRobot().getTime() ;
				}
				else {
					is_done_ = true ;
				}
            }

            void SingleMotorVoltageAction::run() {
				if (timed_ && getSubsystem().getRobot().getTime() - start_ > duration_) {
                	getSubsystem().setMotor(0) ;
					is_done_ = true ;
				}
            }

            bool SingleMotorVoltageAction::isDone() {
                return is_done_ ;
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
