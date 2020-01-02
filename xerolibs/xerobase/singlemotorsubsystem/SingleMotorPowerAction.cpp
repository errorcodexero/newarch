#include "SingleMotorPowerAction.h"
#include "SingleMotorSubsystem.h"
#include "Robot.h"

using namespace xero::misc ;

namespace xero {
    namespace base {
        SingleMotorPowerAction::SingleMotorPowerAction(SingleMotorSubsystem &single_motor_subsystem, double v) : SingleMotorSubsystemAction(single_motor_subsystem)
        {
            power_ = v;
            timed_ = false;
        }

        SingleMotorPowerAction::SingleMotorPowerAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name) : SingleMotorSubsystemAction(single_motor_subsystem) {
            power_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(name) ;
            timed_ = false ;
        }

        SingleMotorPowerAction::SingleMotorPowerAction(SingleMotorSubsystem &single_motor_subsystem, double v, double d) : SingleMotorSubsystemAction(single_motor_subsystem) {
            power_ = v ;
            timed_ = true ;
            duration_ = d ;
        }

        SingleMotorPowerAction::SingleMotorPowerAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name, 
                                                        const std::string &durname) : SingleMotorSubsystemAction(single_motor_subsystem) {
            power_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(name) ;
            timed_ = true ;
            duration_ = single_motor_subsystem.getRobot().getSettingsParser().getDouble(durname) ;
        }           

        SingleMotorPowerAction::~SingleMotorPowerAction() {
        }

        double SingleMotorPowerAction::getElapsed()
        {
            return getSubsystem().getRobot().getTime() - start_;
        }

        void SingleMotorPowerAction::start() {
            SingleMotorSubsystemAction::start();

            auto &logger = getSubsystem().getRobot().getMessageLogger() ;
            setMotor(power_) ;
            if (timed_) {
                logger.startMessage(MessageLogger::MessageType::debug, getID()) ;
                logger << getSubsystem().getName() << ": SingleMotorPowerAction" ;
                logger << ", time " << duration_ << ", power " << power_ ;
                logger.endMessage() ;
                start_ = getSubsystem().getRobot().getTime() ;
            }
            else {
                logger.startMessage(MessageLogger::MessageType::debug, getID()) ;
                logger << getSubsystem().getName() << ": SingleMotorPowerAction" ;
                logger << ", power " << power_ ;
                logger.endMessage() ;
                setDone();
            }
        }

        void SingleMotorPowerAction::run() {
            SingleMotorSubsystemAction::run();

            if (timed_ && getSubsystem().getRobot().getTime() - start_ > duration_) {
                std::vector<double> data;
                auto &logger = getSubsystem().getRobot().getMessageLogger() ;                   
                logger.startMessage(MessageLogger::MessageType::debug, getID()) ;
                logger << getSubsystem().getName() << ": SingleMotorPowerAction complete" ;
                logger.endMessage() ;                       
                setMotor(0) ;
                setDone();
            }
        }

        void SingleMotorPowerAction::cancel() {
            SingleMotorSubsystemAction::cancel();
            setMotor(0) ;            
            setDone();
        }

        std::string SingleMotorPowerAction::toString() {
            std::string result = "SingleMotorPowerAction " ;
            result += std::to_string(power_) ;
            return result ;
        }
    }
}
