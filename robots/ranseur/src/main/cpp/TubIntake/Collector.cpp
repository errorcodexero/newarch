#include "Collector.h"
#include "CollectorAction.h"
#include "Intake.h"
#include <SequenceAction.h>
#include <Robot.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        Collector::Collector(Robot &robot) : Subsystem(robot, "collector") {            
            intake_ = std::make_shared<Intake>(robot) ;
            addChild(intake_) ;
            intake_->createNamedSequences() ;

            int sensor = robot.getSettingsParser().getInteger("hw:collector:tubsensor") ;
            sensor_ = std::make_shared<frc::DigitalInput>(sensor) ;

            double dh2l = robot.getSettingsParser().getDouble("collector:tubsensor:h2ldelay") ;
            double dl2h = robot.getSettingsParser().getDouble("collector:tubsensor:l2hdelay") ;
            deb_sensor_ = std::make_shared<DebounceBoolean>(true, dh2l, dl2h) ;

        }

        Collector::~Collector() {
        }

        bool Collector::canAcceptAction(ActionPtr action) {
            auto coldir_p = std::dynamic_pointer_cast<CollectorAction>(action) ;
            if (coldir_p == nullptr)
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
