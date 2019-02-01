#include "CargoIntake.h"
#include "Robot.h"




using namespace xero::base ;

namespace xero {
    namespace phaser {
        CargoIntake::CargoIntake(xero::base::Robot &robot, uint64_t id, bool victor) : SingleMotorSubsystem(robot, "CargoIntake", "hw:cargointake:motor", id, victor=false){
            solenoid_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:cargointake:solenoid"));
        }
        CargoIntake::~CargoIntake(){

        }
        void CargoIntake::deployCollector(){
            solenoid_->Set(true) ;
        }
        void CargoIntake::retractCollector(){
            solenoid_->Set(false) ;
        }

    }
}
