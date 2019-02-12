#include "HatchIntake.h"
#include "Robot.h"
#include "HatchIntakeAction.h"




using namespace xero::base ;

namespace xero {
    namespace phaser {
        HatchIntake::HatchIntake(xero::base::Robot &robot, uint64_t id, bool victor) : SingleMotorSubsystem(robot, "HatchIntake", "hw:hatchintake:motor", id, victor=false){
            solenoid_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchintake:solenoid"));
            sensor_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:hatchintake:sensor"));

        }
        HatchIntake::~HatchIntake(){

        }
        void HatchIntake::deployCollector(){
            solenoid_->Set(true) ;
        }
        void HatchIntake::retractCollector(){
            solenoid_->Set(false) ;
        }

        bool HatchIntake::HatchCollected(){
            return sensor_->Get() ;
        }

        bool HatchIntake::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<HatchIntakeAction>(action) ;
            return coll != nullptr || SingleMotorSubsystem::canAcceptAction(action) ;            

        }  

    }
}

