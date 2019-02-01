#include "CargoHolder.h"
#include <Robot.h>
#include <ActionSequence.h>
#include <MessageLogger.h>
#include "CargoHolderAction.h"


using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        CargoHolder::CargoHolder(xero::base::Robot &robot) : Subsystem(robot, "CargoHolder") {
            sensor_ = std::make_shared<frc::DigitalInput>(robot.getSettingsParser().getInteger("hw:cargoholder:sensor"));
            motor_ = std::make_shared<VictorSPX>(robot.getSettingsParser().getInteger("hw:cargoholder:motor"));
        }   

        CargoHolder::~CargoHolder() {
        }   

        bool CargoHolder::canAcceptAction(xero::base::ActionPtr action) {
            auto coll = std::dynamic_pointer_cast<CargoHolderAction>(action) ;
            return coll != nullptr ;            

        }  

        void CargoHolder::computeState() {
            has_cargo_ = sensor_->Get() ;
        }
    }
}
