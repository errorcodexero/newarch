#include "CargoIntake.h"
#include "CargoIntakeAction.h"
#include "Robot.h"
#include "CargoIntakeAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CargoIntake::CargoIntake(xero::base::Robot &robot, uint64_t id, bool victor) : SingleMotorSubsystem(robot, "CargoIntake", "hw:cargointake:motor", id, victor){
            solenoid_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:cargointake:solenoid"));
            solenoid_->Set(false) ;          
            sensor_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:cargointake:sensor")) ;  
            has_cargo_ = false ;
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
            if (SingleMotorSubsystem::canAcceptAction(act))
                return true ;
                
            std::shared_ptr<CargoIntakeAction> cargoact = std::dynamic_pointer_cast<CargoIntakeAction>(act) ;
            return cargoact != nullptr || SingleMotorSubsystem::canAcceptAction(act) ;
        }

        void CargoIntake::computeState() {
            is_deployed_ = solenoid_->Get() ;
            has_cargo_ = !sensor_->Get() ;
        }
    }
}
