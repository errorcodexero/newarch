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
            /// \param ids The motor IDs.
            /// \param type The motor type.
            CTREMotorController(const std::list<int> &ids, Type type);

            ~CTREMotorController() { set(0); }

            /// \brief Returns whether these motors are Talon or Victor motors.
            Type getType() { return type_; }

            virtual void set(double percent);

            virtual void setInverted(bool inverted);

            virtual void setNeutralMode(NeutralMode neutralMode);
        private:
            std::list<MotorPtr> motors_;
            Type type_;
        };
    }
}