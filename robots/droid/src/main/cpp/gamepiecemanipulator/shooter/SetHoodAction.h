#pragma once

#include "ShooterAction.h"
#include "Shooter.h"

namespace xero {
    namespace droid {
        class SetHoodAction: public ShooterAction {
        public:
            SetHoodAction(Shooter &sub, Shooter::HoodPosition hood): ShooterAction(sub), hood_(hood) {}

            void start() override {
                ShooterAction::start();
                getSubsystem().setHood(hood_);
                setDone();
            }

            std::string toString() override 
            { 
                std::string ret = "SetHoodAction " ;
                if (hood_ == Shooter::HoodPosition::Up)
                    ret += "UP" ;
                else
                    ret += "DOWN" ;

                return ret ;
            }
            
        private:
            Shooter::HoodPosition hood_;
        };
    }
}