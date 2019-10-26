#include "Collector.h"
#include "CollectorAction.h"
#include <actions/SequenceAction.h>
#include <Robot.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        Collector::Collector(Subsystem *parent) : Subsystem(parent, "collector") {
            Robot &robot = getRobot();
            
            int sensor = robot.getSettingsParser().getInteger("hw:collector:tubsensor") ;
            sensor_ = std::make_shared<frc::DigitalInput>(sensor) ;

            double dh2l = robot.getSettingsParser().getDouble("collector:tubsensor:h2ldelay") ;
            double dl2h = robot.getSettingsParser().getDouble("collector:tubsensor:l2hdelay") ;
            deb_sensor_ = std::make_shared<DebounceBoolean>(true, dh2l, dl2h) ;

            intake_ = robot.getMotorFactory()->createMotor("hw:tubcollector") ;
            intake_->setNeutralMode(MotorController::NeutralMode::Brake) ;

            int clamp = robot.getSettingsParser().getInteger("hw:collector:clamp") ;
            clamp_ = std::make_shared<frc::Solenoid>(clamp) ;

        }

        Collector::~Collector() {
        }

        // sees whether or not 
        bool Collector::canAcceptAction(ActionPtr action) {
            auto col_act_p = std::dynamic_pointer_cast<CollectorAction>(action) ;
            if (col_act_p == nullptr)
                return false ;

            return true ;
        }

        void Collector::computeState() {
            Subsystem::computeState() ;

            deb_sensor_->update(sensor_->Get(), getRobot().getTime()) ;
            has_tub_ = deb_sensor_->get() ;
            if (!has_tub_)
                collected_tub_ = false ;
        }        
    }
}
