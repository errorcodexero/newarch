#pragma once
#include "MotorController.h"

#include <list>
#include <memory>
#include <rev/CANSparkMax.h>

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
        private:
            MotorPtr motor_;
        };
    }
}