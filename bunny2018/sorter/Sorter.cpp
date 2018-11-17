#include "Sorter.h"
//#include "SorterAction.h"
#include <Robot.h>

using namespace xero::base;

namespace xero {
    namespace bunny2018 {
        Sorter::Sorter(xero::base::Robot & robot) :Subsystem(robot,"sorter"){
            int motor=robot.getSettingsParser().getInteger("hw:sorter:motor");
            int enc1=robot.getSettingsParser().getInteger("hw:sorter:encoder1");
            int enc2=robot.getSettingsParser().getInteger("hw:sorter:encoder2");

            motor_ = std::make_shared<frc::VictorSP>(motor);
            encoder_ = std::make_shared<frc::Encoder>(enc1,enc2);

            min_angle_=robot.getSettingsParser().getDouble("sorter:angle:minimum");
            max_angle_=robot.getSettingsParser().getDouble("sorter:angle:maximum");
            degrees_per_tick_=robot.getSettingsParser().getDouble("sorter:degrees_per_tick");

            calibrated_ = false;
        }

        Sorter::~Sorter(){
        }
		
        void Sorter::computeState(){
            encoder_ticks_ = encoder_->Get();
            if(calibrated_){
                angle_ = encoder_ticks_*degrees_per_tick_;
            }
        }

		bool Sorter::canAcceptAction(ActionPtr action) {
			std::shared_ptr<SorterAction> act_p = std::dynamic_pointer_cast<SorterAction>(action) ;
			if (act_p == nullptr)
				return false ;

			// TODO: reject SorterToAngleAction if not calibrated

			return true ;
		}
    }
}