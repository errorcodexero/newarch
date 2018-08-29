#pragma once

#include <cassert>
#include <memory>
#include "DigitalInput.h"
#include "Subsystem.h" 
#include "intake/Intake.h"
#include "grabber/Grabber.h"
#include "DebounceBoolean.h"

namespace xero {
    namespace phoenix {
        class Collector : public xero::base::Subsystem {
            friend class CollectorEjectCubeAction;
            friend class CollectorGetCubeAction;

        public:
            Collector(xero::base::Robot & robot);
            virtual ~Collector(); 
            virtual void computeState();
            virtual bool hasCube(){
                return has_cube_;
            }
            std::shared_ptr<Intake> getIntake(){
                return intake_;
            }
            std::shared_ptr<Grabber> getGrabber(){
                return grabber_;
            }


        private:

            std::shared_ptr<Intake> intake_;
            std::shared_ptr<Grabber> grabber_;
            std::shared_ptr<frc::DigitalInput> sensor_;
            std::shared_ptr<xero::misc::DebounceBoolean> debounce_;
            bool has_cube_;
        };
    }
}