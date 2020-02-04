#pragma once

#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include <actions/Action.h>
#include <SettingsParser.h>
#include <motors/MotorController.h>

#include "Conveyor.h"

namespace xero {
    namespace droid {
        class Conveyor;
        /// Because of the sequential nature of conveyor actions, ConveyorAction
        /// implements a generic state-machine-based framework for controlling
        /// conveyors. A conveyor action is modeled as a sequence of states,
        /// such as "start motors" or "wait for sensor", each implemented by
        /// a \c std::function returning a StateResult
        /// \see ConveyorAction::setStates, ConveyorAction::StateResult 
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
                subsystem_.conveyor_belts_->set(value) ;
            }

            void setMotor2(double value) {
                subsystem_.turret_conveyor_->set(value) ;
            }

        private:
            Conveyor &subsystem_ ;
        } ;
    }
}
