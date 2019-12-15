#pragma once
#include "MotorController.h"

#include <list>
#include <memory>
#include <frc/VictorSP.h>

namespace xero {
    namespace base {
        class VictorSPMotorController: public MotorController {
        public:
            typedef std::shared_ptr<frc::VictorSP> MotorPtr;
        
            /// \brief Creates a new Spark Max motor object
            /// \param canID The motor ID.
            /// \param brushless Whether this motor is brushless.
            VictorSPMotorController(int pwmID);
            virtual ~VictorSPMotorController() ;

            virtual void set(double percent);
            virtual void setInverted(bool inverted);
            virtual void setNeutralMode(NeutralMode neutralMode);
            virtual void follow(std::shared_ptr<MotorController> motor, bool invert = false);
            virtual void reapplyInverted();

        private:
            MotorPtr motor_;
            bool isInverted_;
        };
    }
}