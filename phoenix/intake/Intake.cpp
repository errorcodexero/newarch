#include "Intake.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {
        Intake::Intake(Robot & robot):Subsystem(robot,"intake") {
           int m1 = 3;
           int m2 = 4;

           motor1_ =std::make_shared<frc::VictorSP>(m1);
           motor2_ =std::make_shared<frc::VictorSP>(m2);
        }
        Intake::~Intake(){
            
        }
        void Intake::run(){

        }
        void Intake::computeState(){

        }
    }
}
