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
            Conveyor &getSubsystem() {
                return subsystem_ ;
            }
            
            void start() override;
            void run() override;

            int getStateIndex() { return stateIndex_; }
            
        protected:
            ConveyorAction(Conveyor &subsystem);

            struct StateResult {
                friend class ConveyorAction;
            public:
                static const StateResult Continue;
                static const StateResult Next;
                static StateResult Goto(std::string name) { return StateResult(name); }
            private:
                struct _Continue {};
                struct _Next {};
                std::variant<_Continue, _Next, std::string> value_;

                StateResult(std::variant<_Continue, _Next, std::string> value): value_(value) {}
            };

            // Helper struct to represent a state declaration.
            struct _StateDecl {
                friend class ConveyorAction;
            public:
                _StateDecl(std::function<StateResult(void)> state): 
                    value_(state) {}

                _StateDecl(std::string name, std::function<StateResult(void)> state):
                    value_(std::pair(name, state)) {}
            private:
                // A state function, with or without a name.
                std::variant<std::function<StateResult(void)>,
                             std::pair<std::string, std::function<StateResult(void)>>
                            > value_;
            };

            void setStates(std::vector<_StateDecl> states);

            void setMotor(std::optional<Conveyor::Direction> direction);
            std::function<StateResult(void)> setMotorState(std::optional<Conveyor::Direction> direction);

            /// Creates a state that waits for a sensor to reach a specific value.
            /// \param sensor The sensor to wait for.
            /// \param value The value to wait for.
            /// \param nextState The state to transition to, or -1 to indicate the next state.
            std::function<StateResult(void)> waitForSensorState(Conveyor::SensorPtr sensor, bool value);

            // Creates a state that immediately jumps to the given named state.
            std::function<StateResult(void)> gotoState(std::string name) {
                return [=]() { return StateResult::Goto(name); };
            }

        private:
            Conveyor &subsystem_ ;

            std::vector<std::function<StateResult(void)>> states_;
            std::map<std::string, int> namedStates_;

            void setStateIndex(int stateIndex);

            int stateIndex_;
        } ;
    }
}