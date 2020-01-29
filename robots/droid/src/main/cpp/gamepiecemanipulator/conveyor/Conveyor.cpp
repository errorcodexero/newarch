#include "Conveyor.h"
#include "droidids.h"

#include <utility>
#include <vector>

#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <SettingsParser.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        Conveyor::Conveyor(Subsystem *parent): SingleMotorSubsystem(parent, "conveyor", MSG_GROUP_CONVEYOR) {
            std::vector<std::pair<Sensor, std::string>> sensorNames {
                {Sensor::A, "a"},
                {Sensor::B, "b"},
                {Sensor::C, "c"},
            };
            auto &settings = getRobot().getSettingsParser();
            for (unsigned i = 0; i < sensorNames.size(); i++) {
                int sensorIndex = static_cast<int>(sensorNames[i].first);
                sensors_[sensorIndex] = createSensor(settings, "hw:conveyor:sensor" + sensorNames[i].second);
            }
        }

        void Conveyor::postHWInit() {
            setDefaultAction(std::make_shared<SingleMotorPowerAction>(*this, 0.0));
        }


        Conveyor::SensorPtr Conveyor::createSensor(int channel) {
            return std::make_shared<frc::DigitalInput>(channel);
        }

        Conveyor::SensorPtr Conveyor::createSensor(SettingsParser &settings, std::string configName) {
            assert(settings.isDefined(configName));
            return createSensor(settings.getInteger(configName));
        }

        void Conveyor::setMotor(std::optional<Direction> direction) {
            if (direction) {
                switch (*direction) {
                case Direction::TowardsIntake:
                    SingleMotorSubsystem::setMotor(-1);   // TODO: this should probably be read 
                                                          // from the params file or something
                    break;
                case Direction::TowardsShooter:
                    SingleMotorSubsystem::setMotor(1);    // "
                    break;
                }
            } else {
                SingleMotorSubsystem::setMotor(0);
            }
        }
    }
}