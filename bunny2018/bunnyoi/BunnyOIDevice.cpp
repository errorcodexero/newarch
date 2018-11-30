#include "BunnyOIDevice.h"
#include "BunnyOISubsystem.h"
#include "Bunny.h"
#include "bunnysubsystem/BunnySubsystem.h"
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
            collector_on_off_ = mapButton(11, OIButton::ButtonType::Level) ;            // Toggle
            hopper_on_off_ = mapButton(16, OIButton::ButtonType::Level) ;       		// Toggle

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

			collector_on_action_ = std::make_shared<SingleMotorVoltageAction>(*collector, "collector:power") ;
			collector_off_action_ = std::make_shared<SingleMotorVoltageAction>(*collector, 0.0) ;
			hopper_on_action_ = std::make_shared<SingleMotorVoltageAction>(*hopper, "hopper:power") ;
			hopper_off_action_ = std::make_shared<SingleMotorVoltageAction>(*hopper, 0.0) ;
		}

        void BunnyOIDevice::computeState(ActionSequence &seq) {
			Bunny &bunny = dynamic_cast<Bunny &>(getSubsystem().getRobot()) ;
			auto collector = bunny.getBunnySubsystem()->getCollector() ;
			auto hopper = bunny.getBunnySubsystem()->getHopper() ;

			if (getValue(collector_on_off_) && !collector->isRunning())
				seq.pushSubActionPair(collector, collector_on_action_) ;
			else if (!getValue(collector_on_off_) && collector->isRunning())
				seq.pushSubActionPair(collector, collector_off_action_) ;

			if (getValue(hopper_on_off_) && !hopper->isRunning())
				seq.pushSubActionPair(hopper, hopper_on_action_) ;
			else if (!getValue(hopper_on_off_) && hopper->isRunning())
				seq.pushSubActionPair(hopper, hopper_off_action_) ;				
		}
    }
}