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
        /// Because of the sequential nature of conveyor actions, ConveyorAction
        /// implements a generic state-machine-based framework for controlling
        /// conveyors. A conveyor action is modeled as a sequence of states,
        /// such as "start motors" or "wait for sensor", each implemented by
        /// a \c std::function returning a StateResult
        /// \see ConveyorAction::setStates, ConveyorAction::StateResult 
        class ConveyorAction : public xero::base::Action {
        public:
            Conveyor &getSubsystem() {
                return subsystem_ ;
            }
            
            void start() override;
            void run() override;

            int getStateIndex() { return stateIndex_; }
            
        protected:
            /// Creates a conveyor action.
            /// \param subsystem The conveyor.
            /// Subclasses should call setStates from their constructor.
            ConveyorAction(Conveyor &subsystem);

            /// The result of executing a single tick of a state.
            struct StateResult {
                friend class ConveyorAction;
            public:

                /// Keeps the state machine in the current state.
                static const StateResult Continue;

                /// Transitions the state machine to the next state.
                static const StateResult Next;

                /// Transitions the state machine to a specific named state.
                /// \param name The name of the state to switch to. 
                static StateResult Goto(std::string name) { return StateResult(name); }
            private:
                struct _Continue {};
                struct _Next {};
                std::variant<_Continue, _Next, std::string> value_;

                StateResult(std::variant<_Continue, _Next, std::string> value): value_(value) {}
            };

            /// Helper struct to represent a state declaration.
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
            
            /// Initializes the state machine to the given states.
            /// \param states The list of states. Each state optionally have a name.
            /// Example:
            ///
            /// setStates({
            ///     waitForSensorState(..., true),
            ///     setMotorState(Conveyor::Direction::TowardShooter),
            ///     { "named_state", waitForSensorState(..., false) },
            ///     
            ///     // custom state accepting a parameter
            ///     { std::bind(&MyConveyorAction::myCustomState, this, someParam) },
            ///     
            ///     // same thing as above, but with a lambda
            ///     { [=]() { return myCustomState(someParam); } },
            ///
            ///     { "named_custom_state", std::bind(&MyConveyorAction::otherCustomState, this) }
            /// });
            void setStates(std::vector<_StateDecl> states);

            /// Sets the motor to run in the specified direction.
            void setMotor(std::optional<Conveyor::Direction> direction){ 
                getSubsystem().setMotor(direction);
            }

            /// Creates a state which starts running the motor in the specified direction.
            /// The returned state calls setMotor and immediately returns ::Next.
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