#include "Collector.h"
#include "CollectorAction.h"
#include "grabber/Grabber.h"
#include "phoenixids.h"
#include <actions/SequenceAction.h>
#include <Robot.h>
#include <frc/DigitalInput.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phoenix {
        Collector::Collector(Subsystem *parent) : Subsystem(parent, "collector") {   
            auto &robot = parent->getRobot();
                     
            intake_ = std::make_shared<SingleMotorSubsystem>(this, "intake", "hw:intake", MSG_GROUP_INTAKE) ;
            addChild(intake_) ;

            grabber_ = std::make_shared<Grabber>(this);
            addChild(grabber_) ;

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

            deb_sensor_->update(sensor_->Get(), getRobot().getTime()) ;
            has_cube_ = deb_sensor_->get() ;
            if (!has_cube_)
                collected_cube_ = false ;
        }
    }
}
