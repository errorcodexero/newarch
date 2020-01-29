#include "ConveyorAction.h"
#include "Conveyor.h"

#include <Robot.h>

using namespace xero::misc;

namespace xero {
    namespace droid {
        const ConveyorAction::StateResult ConveyorAction::StateResult::Continue = StateResult(_Continue());
        const ConveyorAction::StateResult ConveyorAction::StateResult::Next = StateResult(_Next());

        ConveyorAction::ConveyorAction(Conveyor &subsystem): 
            Action(subsystem.getRobot().getMessageLogger()), subsystem_(subsystem) {}

        void ConveyorAction::setStates(std::vector<_StateDecl> states) {
            states_.clear();
            states_.reserve(states.size());
            namedStates_.clear();
            for (auto state : states) {
                std::optional<std::string> name = state.value_.first;
                auto func = state.value_.second;
                if (name) {
                    namedStates_.insert({*name, states_.size()});
                }
                states_.push_back(state.value_.second);
            }
        }

        std::function<ConveyorAction::StateResult(void)> 
        ConveyorAction::waitForSensorState(Conveyor::SensorPtr sensor, bool value) {
            return [=]() {
                if (sensor->Get() == value) return StateResult::Next;
                else                        return StateResult::Continue;
            };
        }

        std::function<ConveyorAction::StateResult(void)> 
        ConveyorAction::setMotorState(std::optional<Conveyor::Direction> direction) {
            return [=]() {
                setMotor(direction);
                return StateResult::Next;
            };
        }

        void ConveyorAction::setStateIndex(int stateIndex)  { 
            assert(stateIndex >= 0 && stateIndex < (int)states_.size() && "state index out of range");
            stateIndex_ = stateIndex;
        }

        void ConveyorAction::start() {
            assert(!states_.empty() && "state machine doesn't have any states; call setStates in constructor");
            stateIndex_ = 0; 
        }

        void ConveyorAction::run() {
            while (true) {
                StateResult result = states_[stateIndex_]();

                if (std::holds_alternative<StateResult::_Continue>(result.value_)) {
                    // the state didn't finish, run it again next tick
                    break;
                } else if (std::holds_alternative<StateResult::_Next>(result.value_)) {
                    // go to the next state
                    if (stateIndex_ == (int)(states_.size() - 1)) {
                        // if we finished the last state, we're done
                        setDone();
                        break;
                    } else {
                        setStateIndex(stateIndex_ + 1);
                    }
                } else {
                    // go to a named state
                    std::string name = std::get<std::string>(result.value_);
                    auto it = namedStates_.find(name);
                    assert(it != namedStates_.end() && "state name not found");
                    setStateIndex(it->second);
                }
                // We changed states, so run again
            }
        }
        
    }
}