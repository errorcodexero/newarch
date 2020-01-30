#pragma once

#include <utility>
#include <variant>
#include <vector>

#include <actions/Action.h>
#include <SettingsParser.h>

#include "Conveyor.h"

namespace xero {
    namespace droid {
        class Conveyor;
        class ConveyorAction : public xero::base::Action {
        public:
            ConveyorAction(Conveyor &subsystem);
            virtual ~ConveyorAction() ;

            Conveyor &getSubsystem() {
                return subsystem_ ;
            }
            
            void start() override;
            void run() override;

            void setMotor(double value) {
            }

        private:
            Conveyor &subsystem_ ;
        } ;
    }
}