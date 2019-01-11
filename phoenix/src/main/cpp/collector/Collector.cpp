#include "Collector.h"
#include "CollectorAction.h"
#include "intake/Intake.h"
#include "grabber/Grabber.h"
#include <ActionSequence.h>
#include <Robot.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        Collector::Collector(Robot &robot) : Subsystem(robot, "collector") {            
            intake_ = std::make_shared<Intake>(robot) ;
            addChild(intake_) ;
            intake_->createNamedSequences() ;

            grabber_ = std::make_shared<Grabber>(robot);
            addChild(grabber_) ;
            grabber_->createNamedSequences() ;

            int sensor = robot.getSettingsParser().getInteger("hw:collector:cubesensor") ;
            sensor_ = std::make_shared<frc::DigitalInput>(sensor) ;

            double dh2l = robot.getSettingsParser().getDouble("collector:cubesensor:h2ldelay") ;
            double dl2h = robot.getSettingsParser().getDouble("collector:cubesensor:l2hdelay") ;
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

            has_cube_ = deb_sensor_->getState(sensor_->Get(), getRobot().getTime()) ;
            if (!has_cube_)
                collected_cube_ = false ;
        }
    }
}
