#include "Robot.h"
#include "collector/Collector.h"
#include "collector/CollectorAction.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {

        Collector::Collector(xero::base::Robot & robot) :Subsystem(robot,"Collector"){

            double h2ldelay=robot.getSettingsParser().getDouble("collector:cubesensor:h2ldelay");
            double l2hdelay=robot.getSettingsParser().getDouble("collector:cubesensor:l2hdelay");
            int cubesensor=robot.getSettingsParser().getInteger("hw:collector:cubesensor");
            
            //
			// Add in the intake subsystem
			//
			intake_ = std::make_shared<Intake>(robot) ;

            grabber_ = std::make_shared<Grabber>(robot) ;

            addChild(intake_);
            addChild(grabber_);

            sensor_ = std::make_shared<frc::DigitalInput>(cubesensor) ;

            debounce_ = std::make_shared<xero::misc::DebounceBoolean>(true, h2ldelay, l2hdelay) ;
        }

        Collector::~Collector(){

        }

        bool Collector::canAcceptAction(ActionPtr action) {
            auto collect_action = std::dynamic_pointer_cast<CollectorAction>(action) ;
            return collect_action != nullptr ;
        }
        
        void Collector::computeState(){
            Subsystem::computeState();
            bool state = sensor_->Get();
            has_cube_ = debounce_->getState(state, getRobot().getTime());            
        }
    }
}
