#include "Robot.h"
#include "collector/Collector.h"

using namespace xero::base;

namespace xero {
    namespace phoenix {

        Collector::Collector(xero::base::Robot & robot) :Subsystem(robot,"Collector"){

            double h2ldelay=robot.getSettingsParser().getDouble("collector:cubesensor:h2ldelay");
            double l2hdelay=robot.getSettingsParser().getDouble("collector:cubesensor:l2hdelay");
            double grab_time=robot.getSettingsParser().getDouble("collector:grab_time");
            double fast=robot.getSettingsParser().getDouble("collector:eject:fast");
            double slow=robot.getSettingsParser().getDouble("collector:eject:slow");
            double eject_time=robot.getSettingsParser().getDouble("collector:eject_time");
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
        
        void Collector::computeState(){
            Subsystem::computeState();
            bool state = sensor_->Get();
            has_cube_ = debounce_->getState(state, getRobot().getTime());            
        }


    }
}
