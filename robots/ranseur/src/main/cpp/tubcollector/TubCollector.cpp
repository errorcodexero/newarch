#include "TubCollector.h"
#include "TubCollectorAction.h"
#include <actions/SequenceAction.h>
#include <Robot.h>
#include <frc/DigitalInput.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        TubCollector::TubCollector(Subsystem *parent) : Subsystem(parent, "tubcollector") {
            Robot &robot = getRobot();
            
            int sensor = robot.getSettingsParser().getInteger("hw:tubcollector:tubsensor") ;
            sensor_ = std::make_shared<frc::DigitalInput>(sensor) ;

            intake1_ = robot.getMotorFactory()->createMotor("hw:tubcollector1") ;
            intake1_->setNeutralMode(MotorController::NeutralMode::Brake) ;

            intake2_ = robot.getMotorFactory()->createMotor("hw:tubcollector2") ;
            intake2_->setNeutralMode(MotorController::NeutralMode::Brake) ;            

            int clamp = robot.getSettingsParser().getInteger("hw:tubcollector:clamp") ;
            clamp_ = std::make_shared<frc::Solenoid>(clamp) ;

        }

        TubCollector::~TubCollector() {
        }

        // sees whether or not 
        bool TubCollector::canAcceptAction(ActionPtr action) {
            auto tub_col_act_p = std::dynamic_pointer_cast<TubCollectorAction>(action) ;
            if (tub_col_act_p == nullptr)
                return false ;

            return true ;
        }

        void TubCollector::computeState() {
            Subsystem::computeState() ;

            has_tub_ = !sensor_->Get() ;
            frc::SmartDashboard::PutBoolean("HasTub", has_tub_) ;
        }        

        void TubCollector::reset() {
            Subsystem::reset() ;
            intake1_->set(0.0) ;
            intake2_->set(0.0) ;
        }
    }
}
