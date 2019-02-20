#include "HatchIntake.h"
#include "Robot.h"
#include "HatchIntakeAction.h"
#include "phaserids.h"
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        HatchIntake::HatchIntake(xero::base::Robot &robot, uint64_t id, bool victor) : SingleMotorSubsystem(robot, "HatchIntake", "hw:hatchintake:motor", id, victor){
            solenoid_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:hatchintake:solenoid"));
            sensor_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:hatchintake:sensor"));
            has_hatch_ = false ;
            deployed_ = false ;

            solenoid_->Set(false) ;
        }
        HatchIntake::~HatchIntake(){

        }
        void HatchIntake::deployCollector(){
            solenoid_->Set(true) ;
            deployed_ = true ;
        }
        void HatchIntake::retractCollector(){
            solenoid_->Set(false) ;
            deployed_ = false ;
        }

        bool HatchIntake::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<HatchIntakeAction>(action) ;
            return coll != nullptr || SingleMotorSubsystem::canAcceptAction(action) ;            
        }  

        void HatchIntake::computeState() {
            has_hatch_ = sensor_->Get() ;


            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_INTAKE) ;
            logger << "HatchIntake:" ;
            logger << " HasHatch " << has_hatch_ ;
            logger.endMessage() ;              
        }

    }
}

