#include "CargoIntake.h"
#include "CargoIntakeAction.h"
#include "Robot.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CargoIntake::CargoIntake(xero::base::Robot &robot, uint64_t id, bool victor) : SingleMotorSubsystem(robot, "CargoIntake", "hw:cargointake:motor", id, true){
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

        bool CargoIntake::canAcceptAction(xero::base::ActionPtr act) {
            std::shared_ptr<CargoIntakeAction> cargoact = std::dynamic_pointer_cast<CargoIntakeAction>(act) ;
            return cargoact != nullptr ;
        }

        void CargoIntake::computeState() {
            is_deployed_ = solenoid_->Get() ;
        }
    }
}
