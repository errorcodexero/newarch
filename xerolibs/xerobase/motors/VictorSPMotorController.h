#pragma once
#include "MotorController.h"
#include <frc/VictorSP.h>
#include <list>
#include <memory>
#include <cassert>


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

        protected:
            void addAsFollower(VictorSPMotorController *motor) {
                followers_.push_back(motor) ;
            }

        private:
            MotorPtr motor_;
            bool isInverted_;
            std::shared_ptr<VictorSPMotorController> following_ ;
            std::list<VictorSPMotorController *> followers_ ;
        };
    }
}