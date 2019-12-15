#include "VictorSPMotorController.h"
#include <cassert>

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
            assert(nullptr == "VictorSP controllers do not support setNeutralMode()") ;
        }

        void VictorSPMotorController::follow(std::shared_ptr<MotorController> motor, bool invert)
        {
            assert(nullptr == "VictorSP controllers do not support follow()") ;
        }

        void VictorSPMotorController::reapplyInverted()
        {
        }
    }
}
