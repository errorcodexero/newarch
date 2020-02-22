#pragma once

#include "Conveyor.h"
#include "ConveyorAction.h"

#include <actions/Action.h>
#include <SettingsParser.h>

#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace xero {
    namespace droid {
        class Conveyor;
        /// Because of the sequential nature of conveyor actions, ConveyorStateAction
        /// implements a generic state-machine-based framework for controlling
        /// conveyors. A conveyor action is modeled as a sequence of states,
        /// such as "start motors" or "wait for sensor", each implemented by
        /// a \c std::function returning a StateResult
        /// \see ConveyorStateAction::setStates, ConveyorStateAction::StateResult 
        class ConveyorStateAction : public ConveyorAction {
        public:
            void start() override;
            void run() override;
            void cancel() override { Action::cancel(); setDone(); }

            int getStateIndex() { return stateIndex_; }

            std::string toString() override;
            
        protected:
            /// Creates a conveyor action.
            /// \param subsystem The conveyor.
            /// \param actionName The name of this action (ex. ConveyorEmitAction).
            /// Subclasses should call setStates from their constructor.
            ConveyorStateAction(Conveyor &subsystem, std::string actionName);

            typedef Conveyor::MotorState MotorState;
            typedef Conveyor::Sensor Sensor;

            void setStagedForCollect(bool staged) { getSubsystem().setStagedForCollect(staged); }
            void setStagedForFire(bool staged) { getSubsystem().setStagedForFire(staged); }
            void setCollecting(bool collecting) { getSubsystem().setCollecting(collecting); }

            /// The result of executing a single tick of a state.
            struct StateResult {
                friend class ConveyorStateAction;
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
                friend class ConveyorStateAction;
            public:
                _StateDecl(std::function<StateResult(void)> state): 
                    value_({std::nullopt, state}) {}

                _StateDecl(std::string name, std::function<StateResult(void)> state):
                    value_({name, state}) {}
            private:
                // A state function, with or without a name.
                std::pair<  std::optional<std::string>, 
                            std::function<StateResult(void)>> value_;
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
            ///     { [=] { return myCustomState(someParam); } },
            ///
            ///     { "named_custom_state", [=] { return myOtherState(); } }
            /// });
            void setStates(std::vector<_StateDecl> states);

            /// Sets the motor to run in the specified direction.
            void setMotors(Conveyor::MotorState state){ 
                getSubsystem().setMotors(state);
            }

            /// Creates a state which starts running the motor in the specified direction.
            /// The returned state calls setMotor and immediately returns ::Next.
            std::function<StateResult(void)> setMotorState(Conveyor::MotorState direction);

            /// Creates a state that waits for a sensor to reach a specific value.
            /// \param sensor The sensor to wait for.
            /// \param value The value to wait for.
            std::function<StateResult(void)> waitForSensorState(Conveyor::Sensor sensor, bool value);

            /// Creates a state that waits for a rising or falling edge on a sensor.
            /// \param sensor The sensor to wait for.
            /// \param value The edge to wait for (true for rising, false for falling)
            /// Equivalent to waitForSensorState(sensor, !value)
            ///  followed by  waitForSensorState(sensor, value)
            std::function<StateResult(void)> waitForSensorEdgeState(Conveyor::Sensor sensor, bool value);

            /// A state that increments the number of collected balls.
            std::function<StateResult(void)> incrementBallsState();

            /// A state that decrements the number of collected balls.
            std::function<StateResult(void)> decrementBallsState();

            /// Creates a state which delays for a given amount of time.
            /// \param time The time in seconds.
            std::function<StateResult(void)> delayState(double time);

            /// A state that jumps to \param target if \param condition evaluates to true.
            std::function<StateResult(void)> branchState(std::string target, std::function<bool()> condition) {
                return [=]() { 
                    if(condition()) return StateResult::Goto(target);
                    else            return StateResult::Next;
                };
            }

            // A state that immediately jumps to the given named state.
            std::function<StateResult(void)> gotoState(std::string name) {
                return [=]() { return StateResult::Goto(name); };
            }

            /// A state that verifies a precondition. If the condition fails,
            /// an error message is printed and the action is terminated.
            std::function<StateResult(void)> assertState(std::function<bool()> condition, std::string message);

            virtual void conveyorActionStarted() {
            }

            virtual void conveyorActionFinished() {                
            }

            virtual void conveyorActionRun() {                
            }

        private:
            std::string actionName_;

            // Returns a debug description of the given state index.
            std::string describeState(int stateIndex);

            std::vector<std::function<StateResult(void)>> states_;
            std::unordered_map<std::string, int> namedStates_;

            void setStateIndex(int stateIndex);

            int stateIndex_;

            // When waiting for a rising edge, this is set
            // once the sensor has gone low (and vice-versa)
            bool readyForSensorEdge_;

            std::optional<double> delayEndTime_;
        } ;
    }
}
