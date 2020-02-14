#include "ConveyorStateAction.h"
#include "Conveyor.h"

#include <Robot.h>

using namespace xero::misc;

namespace xero {
    namespace droid {
        const ConveyorStateAction::StateResult ConveyorStateAction::StateResult::Continue = StateResult(_Continue());
        const ConveyorStateAction::StateResult ConveyorStateAction::StateResult::Next = StateResult(_Next());

        ConveyorStateAction::ConveyorStateAction(Conveyor &subsystem, std::string actionName): 
            ConveyorAction(subsystem), actionName_(actionName) 
        {
        }

        std::string ConveyorStateAction::describeState(int index) {
            std::string stateNameStr = "";
            // See if this state has a name
            for (std::pair<std::string, int> namedState : namedStates_) {
                if (namedState.second == index) {
                    stateNameStr = " '" + namedState.first + "'";
                }
            }
            return std::to_string(index) + stateNameStr;
        }

        std::string ConveyorStateAction::toString() {
            return actionName_ + ": current state " + describeState(stateIndex_);
        }

        void ConveyorStateAction::setStates(std::vector<_StateDecl> states) {
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

        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::waitForSensorState(Conveyor::Sensor sensor, bool value) {
            return [=]() {
                if (getSubsystem().readSensor(sensor) == value)
                    return StateResult::Next;
                else
                    return StateResult::Continue;
            };
        }

        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::waitForSensorEdgeState(Conveyor::Sensor sensor, bool value) {
            return [=]() {
                if (!readyForSensorEdge_) {
                    // Waiting for opposite edge
                    if (getSubsystem().readSensor(sensor) != value) {
                        readyForSensorEdge_ = true;
                    }
                } else {
                    // Waiting for edge
                    if (getSubsystem().readSensor(sensor) == value) {
                        readyForSensorEdge_ = false;
                        return StateResult::Next;
                    }
                }
                return StateResult::Continue;
            };
        }

        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::setMotorState(MotorState direction) {
            return [=]() {
                setMotors(direction);
                return StateResult::Next;
            };
        }

        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::incrementBallsState() {
            return [=]() {
                if (getSubsystem().getBallCount() < Conveyor::MAX_BALLS) {
                    getSubsystem().ballCount_++;

                    auto &logger = getMessageLogger();
                    logger.startMessage(MessageLogger::MessageType::debug);
                    logger << actionName_ << ": Collected a ball; count: ";
                    logger << getSubsystem().ballCount_;
                    logger.endMessage();
                } else {
                    auto &logger = getMessageLogger();
                    logger.startMessage(MessageLogger::MessageType::error);
                    logger << actionName_ << ": incrementBallsState: conveyor overflow";
                    logger.endMessage();
                }
                return StateResult::Next;
            };
        }   

        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::decrementBallsState() {
            return [=]() {
                if (getSubsystem().getBallCount() > 0) {
                    getSubsystem().ballCount_--;

                    auto &logger = getMessageLogger();
                    logger.startMessage(MessageLogger::MessageType::debug);
                    logger << actionName_ << ": Released a ball; count: ";
                    logger << getSubsystem().ballCount_;
                    logger.endMessage();
                } else {
                    auto &logger = getMessageLogger();
                    logger.startMessage(MessageLogger::MessageType::error);
                    logger << actionName_ << ": incrementBallsState: conveyor underflow";
                    logger.endMessage();
                }
                return StateResult::Next;
            };
        }     
        
        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::delayState(double time) {
            return [=]() {
                double currentTime = getSubsystem().getRobot().getTime();
                if (delayEndTime_) {
                    if (currentTime > *delayEndTime_) {
                        delayEndTime_ = std::nullopt;
                        return StateResult::Next;
                    }
                } else {
                    delayEndTime_ = currentTime + time;
                }
                return StateResult::Continue;
            };
        }

        std::function<ConveyorStateAction::StateResult(void)> 
        ConveyorStateAction::assertState(std::function<bool()> condition, std::string message) {
                return [=]() {
                    if (!condition()) {
                        auto &logger = getMessageLogger();
                        logger.startMessage(MessageLogger::MessageType::error);
                        logger << actionName_ << ": assertion failed: " << message << "\n";
                        logger << "Terminating action.";
                        logger.endMessage();
                        getSubsystem().setMotors(MotorState::Stopped);
                        setDone();
                    }
                    return StateResult::Next;
                };
            }   

        void ConveyorStateAction::setStateIndex(int stateIndex)  { 
            assert(stateIndex >= 0 && stateIndex < (int)states_.size() && "state index out of range");
            stateIndex_ = stateIndex;
        }

        void ConveyorStateAction::start() {
            assert(!states_.empty() && "state machine doesn't have any states; call setStates in constructor");
            stateIndex_ = 0;
            delayEndTime_ = std::nullopt;
            readyForSensorEdge_ = false;

            auto &logger = getMessageLogger();
            logger.startMessage(MessageLogger::MessageType::debug);
            logger << actionName_ << ": starting in state 0; states: {\n";
            for (unsigned index = 0; index < states_.size(); index++) {
                logger << "    " << describeState(index) << "\n";
            }
            logger << "}";
            logger.endMessage();

            conveyorActionStarted() ;
        }

        void ConveyorStateAction::run() {
            auto &logger = getMessageLogger();

            bool updated = true;
            while (updated && !isDone()) {
                StateResult result = states_[stateIndex_]();

                if (std::holds_alternative<StateResult::_Continue>(result.value_)) {
                    // the state didn't finish, run it again next tick
                    updated = false;
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

            if (isDone())
                conveyorActionFinished() ;
            else
                conveyorActionRun();
        }
        
    }
}