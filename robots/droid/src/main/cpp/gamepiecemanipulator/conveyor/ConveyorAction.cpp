#include "ConveyorAction.h"
#include "Conveyor.h"

#include <Robot.h>

using namespace xero::misc;

namespace xero {
    namespace droid {
        const ConveyorAction::StateResult ConveyorAction::StateResult::Continue = StateResult(_Continue());
        const ConveyorAction::StateResult ConveyorAction::StateResult::Next = StateResult(_Next());

        ConveyorAction::ConveyorAction(Conveyor &subsystem, std::string actionName): 
            Action(subsystem.getRobot().getMessageLogger()), 
            subsystem_(subsystem), actionName_(actionName) {}

        std::string ConveyorAction::describeState(int index) {
            std::string stateNameStr = "";
            // See if this state has a name
            for (std::pair<std::string, int> namedState : namedStates_) {
                if (namedState.second == index) {
                    stateNameStr = " '" + namedState.first + "'";
                }
            }
            return std::to_string(index) + stateNameStr;
        }

        std::string ConveyorAction::toString() {
            return actionName_ + ": current state " + describeState(stateIndex_);
        }

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

            auto &logger = getMessageLogger();
            logger.startMessage(MessageLogger::MessageType::debug);
            logger << actionName_ << ": starting in state 0; states: {\n";
            for (unsigned index = 0; index < states_.size(); index++) {
                logger << "    " << describeState(index) << "\n";
            }
            logger << "}";
            logger.endMessage();
        }

        void ConveyorAction::run() {
            auto &logger = getMessageLogger();

            while (true) {
                StateResult result = states_[stateIndex_]();

                if (std::holds_alternative<StateResult::_Continue>(result.value_)) {
                    // the state didn't finish, run it again next tick
                    break;
                } else if (std::holds_alternative<StateResult::_Next>(result.value_)) {
                    // go to the next state
                    if (stateIndex_ == (int)(states_.size() - 1)) {
                        logger.startMessage(MessageLogger::MessageType::debug);
                        logger << actionName_ << ": finished final state ";
                        logger << describeState(stateIndex_);
                        logger.endMessage();

                        // if we finished the last state, we're done
                        setDone();
                        break;
                    } else {
                        logger.startMessage(MessageLogger::MessageType::debug);
                        logger << actionName_ << ": finished state ";
                        logger << describeState(stateIndex_);
                        logger << "; transitioning to state " << describeState(stateIndex_ + 1);
                        logger.endMessage();
                        
                        setStateIndex(stateIndex_ + 1);
                    }
                } else {
                    // go to a named state
                    std::string name = std::get<std::string>(result.value_);
                    auto it = namedStates_.find(name);
                    assert(it != namedStates_.end() && "state name not found");


                    logger.startMessage(MessageLogger::MessageType::debug);
                    logger << actionName_ << ": finished state ";
                    logger << describeState(stateIndex_);
                    logger << "; jumping to state " << describeState(it->second);
                    logger.endMessage();

                    setStateIndex(it->second);
                }
                // If we changed states, loop again
            }
        }
        
    }
}