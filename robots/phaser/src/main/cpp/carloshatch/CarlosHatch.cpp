#include "CarlosHatch.h"
#include "CarlosHatchAction.h"
#include "phaserids.h"
#include <Robot.h>
#include <ActionSequence.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CarlosHatch::CarlosHatch(xero::base::Robot &robot) : Subsystem(robot, "CarlosHatch") {
            std::cout << "Started Build" << std::endl ;

            arm_extend_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:carloshatch:arm:extend"));
            arm_retract_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:carloshatch:arm:retract"));
            holder_ =  std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:carloshatch:holder"));
            sensor_ = std::make_shared<frc::AnalogInput>(robot.getSettingsParser().getInteger("hw:carloshatch:sensor"));
            impact_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:carloshatch:impact"));

            arm_extend_->Set(true) ;
            arm_retract_->Set(false) ;
            holder_->Set(false) ;

            arm_deployed_ = false ;
            holder_deployed_ = false ;

            double low2high = robot.getSettingsParser().getDouble("carloshatch:impact:low2high") ;
            double high2low = robot.getSettingsParser().getDouble("carloshatch:impact:high2low") ;
            has_impact_debounced_ = std::make_shared<DebounceBoolean>(false, low2high, high2low) ;

            std::cout << "Constructed" << std::endl ;
        }

        CarlosHatch::~CarlosHatch() {
        }   

        bool CarlosHatch::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<CarlosHatchAction>(action) ;
            return coll != nullptr ;            
        }  

        void CarlosHatch::computeState() {
            double now = getRobot().getTime() ;
            has_impact_debounced_->update(impact_->Get(), now) ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "CarlosHatch:" ;
            logger << " voltage " << sensor_->GetVoltage() ;
            logger << " impact " << impact_->Get() ;
            logger.endMessage() ;       
        }

        void CarlosHatch::run() {
            Subsystem::run() ;
        }

        void CarlosHatch::extendArm() {
            arm_extend_->Set(true) ;
            arm_retract_->Set(false) ;
            arm_deployed_ = true ;
        }

        void CarlosHatch::retractArm() {
            arm_extend_->Set(false) ;
            arm_retract_->Set(true) ;
            arm_deployed_ = false ;
        }

        void CarlosHatch::stopArm() {
            arm_extend_->Set(false) ;
            arm_retract_->Set(false) ;            
        }

        void CarlosHatch::activateHolder() {
            holder_->Set(true) ;
            holder_deployed_ = true ;
        }

        void CarlosHatch::deactivateHolder() {
            holder_->Set(false) ;
            holder_deployed_ = false ;
        }
    }
}
