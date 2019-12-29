#include "VictorSPMotorController.h"
#include <cassert>

namespace xero
{
    namespace base 
    {

        VictorSPMotorController::VictorSPMotorController(int pwmID)
        {
            motor_ = std::make_shared<frc::VictorSP>(pwmID) ;
            isInverted_ = false;
        }

        VictorSPMotorController::~VictorSPMotorController() 
        { 
            set(0); 
        }

        void VictorSPMotorController::set(double percent)
        {
            motor_->Set(isInverted_ ? -percent : percent) ;
            for(auto follower: followers_)
                follower->set(isInverted_ ? -percent : percent) ;
        }

        void VictorSPMotorController::setInverted(bool inverted)
        {
            isInverted_ = inverted ;
        }

        void VictorSPMotorController::setNeutralMode(NeutralMode neutralMode)
        {
            assert(nullptr == "VictorSP controllers do not support setNeutralMode()") ;
        }

        void VictorSPMotorController::follow(std::shared_ptr<MotorController> motor, bool invert)
        {
            auto vspmotor = std::dynamic_pointer_cast<VictorSPMotorController>(motor) ;
            following_ = vspmotor ;
            vspmotor->addAsFollower(this) ;
        }

        void VictorSPMotorController::reapplyInverted()
        {
        }
    }
}
