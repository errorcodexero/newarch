#include <Robot.h>
#include "ShooterStageBallAction.h"

namespace xero {
    namespace bunny2018 {
        
            ShooterStageBallAction::ShooterStageBallAction(Shooter& shooter) : ShooterAction(shooter) {
                is_done_ = false ;
            }

            ShooterStageBallAction::~ShooterStageBallAction() {
            }

            void ShooterStageBallAction::start() {
            }

            void ShooterStageBallAction::run() {
                double motor_power;
                if (getSubsystem().getBallIsStaged()) {
                    motor_power = 0;
                } else {
                    motor_power = getSubsystem().getStageMotorPower();
                }
                getSubsystem().setMotor(motor_power);
                is_done_ = getSubsystem().getBallIsStaged() ;
            }

            bool ShooterStageBallAction::isDone() {
                return is_done_ ;
            }

            void ShooterStageBallAction::cancel() {
                getSubsystem().setMotor(0);
                is_done_ = true ;
            }

            std::string ShooterStageBallAction::toString() {
                std::string result = "ShooterStageBallAction staged=" ;
                if (getSubsystem().getBallIsStaged())
                    result += "true" ;
                else
                    result += "false" ;

                return result ;
            }
    }
}
