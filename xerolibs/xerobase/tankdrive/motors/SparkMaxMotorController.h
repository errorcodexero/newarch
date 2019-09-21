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
            /// \param ids The motor IDs.
            /// \param brushless Whether these motors are brushless.
            SparkMaxMotorController(const std::list<int> &ids, bool brushless);

            ~SparkMaxMotorController() { set(0); }

            virtual void set(double percent);

            virtual void setInverted(bool inverted);

            virtual void setNeutralMode(NeutralMode neutralMode);
        private:
            std::list<MotorPtr> motors_;
        };
    }
}