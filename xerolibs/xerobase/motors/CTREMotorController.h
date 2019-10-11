#pragma once
#include "MotorController.h"

#include <list>
#include <memory>
#include <ctre/Phoenix.h>

namespace xero {
    namespace base {
        class CTREMotorController: public MotorController {
        public:
            typedef std::shared_ptr<ctre::phoenix::motorcontrol::IMotorController> MotorPtr;

            enum class Type { TalonSRX, VictorSPX };
        
            /// \brief Creates a new CTRE motor object
            /// \param canID The motor ID.
            /// \param type The motor type.
            CTREMotorController(int canID, Type type);

            virtual ~CTREMotorController() { set(0); }

            /// \brief Returns whether this motor is a Talon or Victor motor.
            Type getType() { return type_; }

            virtual void set(double percent);
            virtual void setInverted(bool inverted);
            virtual void setNeutralMode(NeutralMode neutralMode);
            virtual void follow(std::shared_ptr<MotorController> motor, bool invert = false);
        private:
            MotorPtr motor_;
            Type type_;
        };
    }
}