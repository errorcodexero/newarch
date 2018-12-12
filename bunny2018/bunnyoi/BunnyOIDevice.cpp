#include "BunnyOIDevice.h"
#include "BunnyOISubsystem.h"
#include "Bunny.h"
#include "bunnysubsystem/BunnySubsystem.h"
#include "sorter/SorterSortAction.h"
#include <ActionSequence.h>
#include <singlemotorsubsystem/SingleMotorSubsystem.h>
#include <singlemotorsubsystem/SingleMotorVoltageAction.h>

using namespace xero::base ;

namespace xero {
    namespace bunny2018 {
        BunnyOIDevice::BunnyOIDevice(BunnyOISubsystem &sub, int index) : OIDevice(sub, index) {
            initialize() ;
        }

        BunnyOIDevice::~BunnyOIDevice() {
        }
        
        void BunnyOIDevice::initialize() {
            //
            // Actions
            //
			collector_ = mapAxisSwitch(4,3) ;
			hopper_ = mapAxisSwitch(3,3) ;
			sorter_color_ = mapButton(15, OIButton::ButtonType::Level) ;
			shoot_one_ = mapButton(3, OIButton::ButtonType::LowToHigh) ;
			shoot_many_ = mapButton(6, OIButton::ButtonType::Level) ;

            //
            // Modes
            //
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(6, mapping) ;                                  // Rotary knob, 10 position
        }

		void BunnyOIDevice::createActions() {
			Bunny &bunny = dynamic_cast<Bunny &>(getSubsystem().getRobot()) ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;
			auto sorter = bunny.getBunnySubsystem()->getSorter() ;

			collector_fwd_action_ = std::make_shared<SingleMotorVoltageAction>(*collector, "collector:power:fwd") ;
			collector_off_action_ = std::make_shared<SingleMotorVoltageAction>(*collector, 0.0) ;
			collector_rev_action_ = std::make_shared<SingleMotorVoltageAction>(*collector, "collector:power:rev") ;
			hopper_fwd_action_ = std::make_shared<SingleMotorVoltageAction>(*hopper, "hopper:power:fwd") ;
			hopper_off_action_ = std::make_shared<SingleMotorVoltageAction>(*hopper, 0.0) ;
			hopper_rev_action_ = std::make_shared<SingleMotorVoltageAction>(*hopper, "hopper:power:rev") ;
			sort_red_ = std::make_shared<SorterSortAction>(*sorter, Sorter::BallColor::Red) ;
			sort_blue_ = std::make_shared<SorterSortAction>(*sorter, Sorter::BallColor::Blue) ;
		}

        void BunnyOIDevice::generateActions(ActionSequence &seq) {
            if (collector_fwd_action_ == nullptr)
                createActions() ;

			Bunny &bunny = dynamic_cast<Bunny &>(getSubsystem().getRobot()) ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;

			switch(getValue(collector_)){
				case 0:
					seq.pushSubActionPair(collector, collector_rev_action_) ;
					break;
				case 1:
					seq.pushSubActionPair(collector, collector_off_action_) ;                
					break;
				case 2:
					seq.pushSubActionPair(collector, collector_fwd_action_) ;
					break;
			}

			switch(getValue(hopper_)){
				case 0:
					seq.pushSubActionPair(hopper, hopper_rev_action_) ;
					break;
				case 1:
					seq.pushSubActionPair(hopper,hopper_off_action_) ;
					break;
				case 2:
					seq.pushSubActionPair(hopper, hopper_fwd_action_) ;                
					break;
			}	
		}
    }
}