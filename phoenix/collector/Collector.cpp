#include "Robot.h"
#include "Collector/Collector.h"
#include "Collector/CollectorAction.h"

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

        bool Collector::canAcceptAction(ActionPtr action) {
            auto ptr = std::dynamic_pointer_cast<CollectorAction>(action) ;
            return ptr != nullptr ;
        }

        Collector::~Collector(){

        }
        
        void Collector::computeState(){
            Subsystem::computeState();
            bool state = sensor_->Get();
            has_cube_ = debounce_->getState(state, getRobot().getTime());            
        }


    }
}