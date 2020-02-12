#pragma once
#include "MotorController.h"

#include <list>
#include <memory>
#include <ctre/Phoenix.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace ctre::phoenix::motorcontrol ;

namespace xero {
    namespace base {
        class CTREMotorController: public MotorController {
        public:
            typedef std::shared_ptr<ctre::phoenix::motorcontrol::IMotorController> MotorPtr;

            enum class Type { TalonSRX, TalonFX, VictorSPX };
        
            /// \brief Creates a new CTRE motor object
            /// \param canID The motor ID.
            /// \param type The motor type.
            CTREMotorController(int canID, Type type);

            virtual ~CTREMotorController() { set(0); }

            virtual void set(double percent);
            virtual void setInverted(bool inverted);
            virtual void setNeutralMode(NeutralMode neutralMode);
            virtual void follow(std::shared_ptr<MotorController> motor, bool invert = false);
            virtual void reapplyInverted();
            virtual std::string getType() {
                std::string type("ctre") ;
                return type ;
            }

            virtual bool hasPosition() {
                return type_ == Type::TalonFX ;
            }

            // Units are 2048/revolution
            virtual int getPosition() {
                assert(type_ == Type::TalonFX);                
                auto talon = std::dynamic_pointer_cast<ctre::phoenix::motorcontrol::can::TalonFX>(motor_);                
                if (!rate_updated_)
                {
                    talon->SetStatusFramePeriod(Status_2_Feedback0_, 10) ;
                    rate_updated_ = true ;
                }
                return static_cast<int>(talon->GetSensorCollection().GetIntegratedSensorPosition());
            }
            
        private:
            MotorPtr motor_;
            Type type_;
            bool isInverted_;
            bool rate_updated_ ;
        };
    }
}