#include "VictorSPMotorController.h"

namespace xero
{
    namespace base 
    {

        VictorSPMotorController::VictorSPMotorController(int pwmID)
        {
        }

        VictorSPMotorController::~VictorSPMotorController() 
        { 
            set(0); 
        }

        void VictorSPMotorController::set(double percent)
        {
            motor_->Set(isInverted_ ? -percent : percent) ;
        }

        void VictorSPMotorController::setInverted(bool inverted)
        {
            isInverted_ = true ;
        }

        void VictorSPMotorController::setNeutralMode(NeutralMode neutralMode)
        {
            assert(false) ;
        }

        void VictorSPMotorController::follow(std::shared_ptr<MotorController> motor, bool invert = false)
        {
            assert(false) ;
        }

        void VictorSPMotorController::reapplyInverted()
        {
        }
    }
}
