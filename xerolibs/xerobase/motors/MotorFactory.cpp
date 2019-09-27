#include "MotorFactory.h"
#include "Robot.h"
#include "MotorController.h"
#include "CTREMotorController.h"
#include "SparkMaxMotorController.h"
#include "MotorGroupController.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        // For each motor type, a function to construct that motor given the CAN ID.
        std::map<std::string, 
                 std::function<MotorFactory::MotorPtr(int)>
                > MotorFactory::motorConstructors_ = {
            { "talon_srx", [](int canID) {
                return CTREMotorController(canID, CTREMotorController::Type::TalonSRX);
            }},
            { "victor_spx", [](int canID) {
                return CTREMotorController(canID, CTREMotorController::Type::VictorSPX);
            }},
            { "sparkmax_brushless", [](int canID) {
                return SparkMaxMotorController(canID, true);
            }},
            { "sparkmax_brushed", [](int canID) {
                return SparkMaxMotorController(canID, false);
            }}
        };

        MotorFactory::MotorFactory(Robot &robot) :
            idsUsed_(), 
            messageLogger_(robot.getMessageLogger()),
            settingsParser_(robot.getSettingsParser()) {}

        void MotorFactory::handleError(std::string configID, std::string message) {
            messageLogger_.startMessage(MessageLogger::MessageType::error);
            messageLogger_ << "Error creating motor " << configID << ": " << message;
            messageLogger_.endMessage();
            assert(0);
        }

        MotorPtr MotorFactory::createSingleMotor(std::string configID) {
            bool hasCanID = settingsParser_.isDefined(configID + ":canid");
            bool hasType = settingsParser_.isDefined(configID + ":type");

            if (!(hasCanID && hasType)) {
                if (hasCanID || hasType) handleError(configID, "must define both canid and type");
                else return nullptr;
            }

            int canID = settingsParser_.getInt(configID + ":canid");
            std::string type = settingsParser_.getString(configID + ":type");
            
            auto constructor = motorConstructors_.find(type);
            if (constructor == motorConstructors_.end()) {
                // The motor type does not exist. List supported motor types and crash.
                std::string message = "unsupported motor type '" + type + "' (supported types:";
                constructor = motorConstructors_.begin();
                while (constructor != motorConstructors_.end()) 
                    message += " '" + constructor->first + "'";
                handleError(configID, message + ")");
            }

            // Ensure CAN ID does not already exist.
            if (idsUsed_.find(canID) != idsUsed_.end()) {
                handleError("duplicate CAN ID " + canID);
            }

            // Create the motor.
            return constructor->second(canID);
        }

        MotorPtr MotorFactory::createMotor(std::string configID) {
            // Try to create & return a single motor.
            if (auto motor = createSingleMotor(configID)) return motor;

            // Could not parse as a single motor declaration.
            // Try to create a motor group instead.
            std::vector motors;
            while (true) {
                // Try to create the next motor.
                if (auto motor = createSingleMotor(configID + ":" + (motors.size() + 1))) {
                    motors.push_back(motor);
                } else {
                    // Could not create another motor.
                    if (motors.empty()) {
                        // No motors were declared.
                        handleError("invalid motor declaration");
                    } else {
                        // We've reached the end of the motor list.
                        break;
                    }
                }
            }
        }
    }
}
