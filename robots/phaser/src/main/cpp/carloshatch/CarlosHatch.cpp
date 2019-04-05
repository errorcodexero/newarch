#include "CarlosHatch.h"
#include "CarlosHatchAction.h"
#include "phaserids.h"
#include <Robot.h>
#include <ActionSequence.h>
#include <MessageLogger.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CarlosHatch::CarlosHatch(xero::base::Robot &robot) : Subsystem(robot, "CarlosHatch") {
            arm_extend_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:carloshatch:arm:extend"));
            arm_retract_ = std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:carloshatch:arm:retract"));
            holder_ =  std::make_shared<frc::Solenoid>(robot.getSettingsParser().getInteger("hw:carloshatch:holder"));
            sensor_ = std::make_shared<frc::AnalogInput>(robot.getSettingsParser().getInteger("hw:carloshatch:sensor"));
            impact_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:carloshatch:impact"));

            hatch_present_threshold_ = robot.getSettingsParser().getDouble("carloshatch:threshold") ;

            arm_extend_->Set(false) ;
            arm_retract_->Set(true) ;
            holder_->Set(false) ;

            arm_deployed_ = false ;
            hooks_enabled_ = false ;

            double low2high = robot.getSettingsParser().getDouble("carloshatch:impact:low2high") ;
            double high2low = robot.getSettingsParser().getDouble("carloshatch:impact:high2low") ;
            fully_extended_debounced_ = std::make_shared<DebounceBoolean>(false, low2high, high2low) ;

            low2high = robot.getSettingsParser().getDouble("carloshatch:presence:low2high") ;
            high2low = robot.getSettingsParser().getDouble("carloshatch:presence:high2low") ;
            has_hatch_debounced_ = std::make_shared<DebounceBoolean>(false, low2high, high2low) ;

            prev_state_ = false ;
        }

        CarlosHatch::~CarlosHatch() {
        }   

        bool CarlosHatch::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<CarlosHatchAction>(action) ;
            return coll != nullptr ;            
        }  

        void CarlosHatch::computeState() {
            double now = getRobot().getTime() ;
            bool hatchpres = false ;
            bool impactval ;

            double pres = sensor_->GetVoltage() ;
            if (pres > hatch_present_threshold_)
                hatchpres = true ;
            else
                hatchpres = false ;            

            impactval = !impact_->Get() ;
            fully_extended_debounced_->update(impactval, now) ;
            has_hatch_debounced_->update(hatchpres, now) ;

            auto &logger = getRobot().getMessageLogger() ;            

            if (prev_state_ != fully_extended_debounced_->get()) {
                prev_state_ = fully_extended_debounced_->get() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
                logger << "CarlosHatch:" ;                
                logger << " impact sensor toggled, state = " << prev_state_ ;
                logger.endMessage() ;
            }

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER_VERBOSE) ;
            logger << "CarlosHatch:" ;
            logger << " voltage " << sensor_->GetVoltage() ;
            logger << " threshold " << hatch_present_threshold_ ;
            logger << " fully extended raw " << impactval ;
            logger << " funny extended deb " << fully_extended_debounced_->get() ;
            logger << " presence raw " << hatchpres ;
            logger << " presence deb " << has_hatch_debounced_->get() ;            
            logger.endMessage() ;  

            frc::SmartDashboard::PutBoolean("Extended", has_hatch_debounced_->get()) ;                 
        }

        void CarlosHatch::run() {
            Subsystem::run() ;
        }

        void CarlosHatch::extendArm() {
            arm_extend_->Set(true) ;
            arm_retract_->Set(false) ;
            arm_deployed_ = true ;
            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "CarlosHatch:" ;                
            logger << " extendArm" ;
            logger.endMessage() ;                 
        }

        void CarlosHatch::retractArm() {
            arm_extend_->Set(false) ;
            arm_retract_->Set(true) ;
            arm_deployed_ = false ;
            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "CarlosHatch:" ;                
            logger << " retractArm" ;
            logger.endMessage() ;                 
        }

        void CarlosHatch::stopArm() {
            arm_extend_->Set(false) ;
            arm_retract_->Set(false) ;            
            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "CarlosHatch:" ;                
            logger << " stopArm" ;
            logger.endMessage() ;                 

            std::cout << "Stop Arm" << std::endl ;
        }

        void CarlosHatch::enableHooks() {
            holder_->Set(false) ;
            hooks_enabled_ = true ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "CarlosHatch:" ;                
            logger << " enable hooks" ;
            logger << " has hatch " << has_hatch_debounced_->get() ;            
            logger.endMessage() ;            
        }

        void CarlosHatch::disableHooks() {
            holder_->Set(true) ;
            hooks_enabled_ = false ;

            auto &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_HATCH_HOLDER) ;
            logger << "CarlosHatch:" ;                
            logger << " disable hooks, " ;
            logger << " has hatch " << has_hatch_debounced_->get() ;
            logger.endMessage() ;              
        }
    }
}
