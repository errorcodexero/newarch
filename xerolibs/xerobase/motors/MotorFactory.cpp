#include "MotorFactory.h"
#include "Robot.h"
#include "MotorController.h"
#include "CTREMotorController.h"
#include "SparkMaxMotorController.h"
#include "MotorGroupController.h"
#include "VictorSPMotorController.h"

using namespace xero::misc;

namespace xero {
    namespace base {
        // For each motor type, a function to construct that motor given the CAN ID.
        std::map<std::string, MotorFactory::MotorDescriptor> MotorFactory::motorConstructors_ = {
            { 
                "talon_srx", 
                {
                    MotorType::CAN,
                    [](int canID) {
                        return std::make_shared<CTREMotorController>(canID, CTREMotorController::Type::TalonSRX);
                    }
                }
            },
            { 
                "talon_fx", 
                {
                    MotorType::CAN,
                    [](int canID) {
                        return std::make_shared<CTREMotorController>(canID, CTREMotorController::Type::TalonFX);
                    }
                }
            },
            { 
                "victor_spx", 
                {
                    MotorType::CAN,
                    [](int canID) {
                        return std::make_shared<CTREMotorController>(canID, CTREMotorController::Type::VictorSPX);
                    }
                }
            },
            { 
                "sparkmax_brushless", 
                {
                    MotorType::CAN,
                    [](int canID) {
                        return std::make_shared<SparkMaxMotorController>(canID, true);
                    }
                }
            },
            { 
                "sparkmax_brushed", 
                {
                    MotorType::CAN,
                    [](int canID) {
                        return std::make_shared<SparkMaxMotorController>(canID, false);
                    }
                }
            },
            { 
                "victorsp", 
                {
                    MotorType::PWM,
                    [](int pwmID) {
                        return std::make_shared<VictorSPMotorController>(pwmID) ;
                    }
                }
            },
        };

        MotorFactory::MotorFactory(Robot &robot) :
            messageLogger_(robot.getMessageLogger()),
            settingsParser_(robot.getSettingsParser()) {}

        void MotorFactory::handleError(std::string configID, std::string message) {
            messageLogger_.startMessage(MessageLogger::MessageType::error);
            messageLogger_ << "Error creating motor " << configID << ": " << message;
            messageLogger_.endMessage();
            assert(0);
        }

        bool MotorFactory::isInverted(std::string configID) {
            std::string invertID = configID + ":invert";
            return settingsParser_.isDefined(invertID) && settingsParser_.getBoolean(invertID);
        }

        MotorFactory::MotorPtr MotorFactory::createSingleMotor(std::string configID) {
            bool hasType = settingsParser_.isDefined(configID + ":type");
            bool hasCanID = settingsParser_.isDefined(configID + ":canid");
            bool hasPWMID = settingsParser_.isDefined(configID + ":pwmid");

            if (!hasType)
            {
                if (hasCanID)
                    handleError(configID, "has canid but not type, must have both") ;

                if (hasPWMID)
                    handleError(configID, "has pwmid but not type, must have both") ;
                
                // This is the case of neither, this happens when we are creating a
                // motor group with multiple motors.
                return nullptr ;
            }

            std::string type = settingsParser_.getString(configID + ":type");
            auto constructor = motorConstructors_.find(type);
            if (constructor == motorConstructors_.end()) {
                // The motor type does not exist. List supported motor types and crash.
                std::string message = "unsupported motor type '" + type + "' (supported types:";
                constructor = motorConstructors_.begin();
                while (constructor != motorConstructors_.end()) 
                {
                    message += " '" + constructor->first + "'";
                    constructor++ ;
                }
                handleError(configID, message + ")");
            }

            MotorPtr motor ;
            if (constructor->second.type_ == MotorType::CAN)
            {
                if (!hasCanID) {
                    std::string msg = "for motor type '" + constructor->first + "' you must define 'canid'" ;
                    handleError(configID, msg) ;
                }

                int canID = settingsParser_.getInteger(configID + ":canid");
                // Ensure CAN ID does not already exist.
                if (canIdsUsed_.find(canID) != canIdsUsed_.end()) {
                    handleError(configID, "duplicate CAN ID " + std::to_string(canID));
                }

                // Create the motor.
                motor = constructor->second.createFunction(canID);
                canIdsUsed_.insert(canID) ;
            }
            else
            {
                if (!hasPWMID) {
                    std::string msg = "for motor type '" + constructor->first + "' you must define 'pwmid'" ;
                    handleError(configID, msg) ;
                }

                int pwmID = settingsParser_.getInteger(configID + ":pwmid");
                // Ensure PWM ID does not already exist.
                if (pwmIdsUsed_.find(pwmID) != pwmIdsUsed_.end()) {
                    handleError(configID, "duplicate PWM ID " + std::to_string(pwmID));
                }                
                // Create the motor.
                motor = constructor->second.createFunction(pwmID) ;
                pwmIdsUsed_.insert(pwmID) ;
            }

            motor->setInverted(isInverted(configID)) ;
            return motor;
        }

        MotorFactory::MotorPtr MotorFactory::createMotor(std::string configID) {
            // Try to create & return a single motor.
            if (auto motor = createSingleMotor(configID)) return motor;

            // Could not parse as a single motor declaration.
            // Try to create a motor group instead.
            MotorGroupController motors;
            int currentIndex = 0;
            bool groupInverted = isInverted(configID);  // True if the group invert flag is set
            bool leaderInverted = false;                // True if the leader's invert flag is set
            while (true) {
                // Try to create the next motor.
                std::string motorConfigID = configID + ":" + std::to_string(currentIndex + 1);
                if (auto motor = createSingleMotor(motorConfigID)) {
                    // we need to catch the invert flag and pass it into follow
                    bool v = isInverted(motorConfigID) ;
                    if (currentIndex == 0) {
                        leaderInverted = v;
                        if (groupInverted) v = !v;  // If the group is inverted, just invert the leader
                        motor->setInverted(v) ;
                    } else if (leaderInverted) v = !v;  // If the leader is inverted, invert all other motors
                                                        // so that they follow the direction of the group
                    motors.add(motor, v);
                    currentIndex += 1;
                } else {
                    // Could not create another motor.
                    if (!currentIndex) {
                        // No motors were declared.
                        handleError(configID, "invalid motor declaration");
                    } else {
                        // We've reached the end of the motor list.
                        break;
                    }
                }
            }

            return std::make_shared<MotorGroupController>(motors);
        }
    }
}
