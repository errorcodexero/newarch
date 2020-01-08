#pragma once
#include "MotorController.h"

#include <list>
#include <memory>
#include <rev/CANSparkMax.h>
#include <rev/CANEncoder.h>

namespace xero {
    namespace base {
        class SparkMaxMotorController: public MotorController {
        public:
            typedef std::shared_ptr<rev::CANSparkMax> MotorPtr;
        
            /// \brief Creates a new Spark Max motor object
            /// \param canID The motor ID.
            /// \param brushless Whether this motor is brushless.
            SparkMaxMotorController(int canID, bool brushless);

            virtual ~SparkMaxMotorController() { set(0); }

            virtual void set(double percent);
            virtual void setInverted(bool inverted);
            virtual void setNeutralMode(NeutralMode neutralMode);
            virtual void follow(std::shared_ptr<MotorController> motor, bool invert = false);
            virtual void reapplyInverted();
            virtual std::string getType() {
                std::string type("sparkmax") ;
                return type ;
            }

            virtual bool hasPosition() {
                return true ;
            }

            virtual int getPosition() {
                return static_cast<int>(encoder_->GetPosition()) ;
            }

            virtual void resetPosition() {
                encoder_->SetPosition(0);
            }

        private:
            MotorPtr motor_;
            bool isInverted_;
            rev::CANEncoder *encoder_ ;
        };
    }
}