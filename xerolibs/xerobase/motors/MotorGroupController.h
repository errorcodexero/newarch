#pragma once
#include "MotorController.h"

#include <vector>
#include <memory>
#include <cassert>

namespace xero {
    namespace base {
        class MotorGroupController: public MotorController {
        public:
            typedef std::shared_ptr<MotorController> MotorPtr;

            /// \brief Creates an empty motor group.
            MotorGroupController();
        
            /// \brief Creates a new motor group
            /// \param motors The motors in the group
            MotorGroupController(std::vector<MotorPtr> motors);

            /// \brief Adds a motor to the group.
            /// \param motor The motor to add.
            /// \param invert Whether the motor should be inverted from the other motors.
            void add(MotorPtr motor, bool invert = false);

            virtual ~MotorGroupController() { set(0); }

            virtual void set(double percent);
            virtual void setInverted(bool inverted);
            virtual void reapplyInverted();
            virtual void setNeutralMode(NeutralMode neutralMode);
            virtual void follow(MotorPtr motor, bool invert = false) {
                assert(0 == "MotorGroupController cannot follow");
            }
        private:
            std::vector<MotorPtr> motors_;
        };
    }
}