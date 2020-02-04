#include "Conveyor.h"
#include "droidids.h"
#include "ConveyorStateAction.h"
#include "ConveyorStopAction.h"

#include <utility>
#include <vector>

#include <Robot.h>
#include <motors/MotorFactory.h>
#include <SettingsParser.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Conveyor::Conveyor(Subsystem *parent): Subsystem(parent, "conveyor") {
            auto motorFactory = getRobot().getMotorFactory();
            intakeMotor_ = motorFactory->createMotor("hw:conveyor:motors:intake");
            shooterMotor_ = motorFactory->createMotor("hw:conveyor:motors:shooter");

            std::vector<std::pair<Sensor, std::string>> sensorNames {
                {Sensor::A, "a"},
                {Sensor::B, "b"},
                {Sensor::C, "c"},
            };
            auto &settings = getRobot().getSettingsParser();
            for (unsigned i = 0; i < sensorNames.size(); i++) {
                int sensorIndex = static_cast<int>(sensorNames[i].first);
                sensors_[sensorIndex] = createSensor(settings, "hw:conveyor:sensor:" + sensorNames[i].second);
            }

            std::vector<std::pair<MotorState, std::string>> motorStates {
                {MotorState::Stopped, "stopped"},
                {MotorState::MoveTowardsShooter, "move_towards_shooter"},
                {MotorState::MoveTowardsIntake, "move_towards_intake"},
            };

            for (auto state : motorStates) {
                assert(static_cast<unsigned>(state.first) == motorStates_.size() &&
                       "motor states must be in order");
                std::string configBase = "conveyor:motorstates:" + state.second;
                std::string intake = configBase + ":intake";
                std::string shooter = configBase + ":shooter";
                if (!settings.isDefined(intake) || !settings.isDefined(shooter)) {
                    auto &logger = getRobot().getMessageLogger();
                    logger.startMessage(MessageLogger::MessageType::error);
                    logger << "Conveyor: motor state " << configBase << " invalid";
                    logger << " (should declare :intake and :shooter)";
                    logger.endMessage();
                    assert(0);
                }
                motorStates_.push_back({
                    settings.getDouble(intake),
                    settings.getDouble(shooter)
                });
            }
        }

        void Conveyor::postHWInit() {
            setDefaultAction(std::make_shared<ConveyorStopAction>(*this));
        }

        bool Conveyor::canAcceptAction(ActionPtr action) {
            return std::dynamic_pointer_cast<ConveyorAction>(action) != nullptr;
        }

        bool Conveyor::canAcceptDefaultAction(ActionPtr action) {
            return std::dynamic_pointer_cast<ConveyorStopAction>(action) != nullptr;
        }


        Conveyor::SensorPtr Conveyor::createSensor(int channel) {
            return std::make_shared<frc::DigitalInput>(channel);
        }

        Conveyor::SensorPtr Conveyor::createSensor(SettingsParser &settings, std::string configName) {
            assert(settings.isDefined(configName));
            return createSensor(settings.getInteger(configName));
        }

        void Conveyor::setMotors(MotorState state) {
            unsigned index = static_cast<unsigned>(state);
            assert(index < motorStates_.size() && "a motor state is missing");
            auto speeds = motorStates_[index];

            intakeMotor_->set(speeds.first);
            shooterMotor_->set(speeds.second);
        }
    }
}
