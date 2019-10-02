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
        
            /// \brief Creates a new motor group
            /// \param motors The motors in the group
            MotorGroupController(std::vector<MotorPtr> motors);

            virtual ~MotorGroupController() { set(0); }

            virtual void set(double percent);
            virtual void setInverted(bool inverted);
            virtual void setNeutralMode(NeutralMode neutralMode);
            virtual void follow(MotorPtr motor) {
                assert(0 == "MotorGroupController cannot follow");
            }
        private:
            std::vector<MotorPtr> motors_;
        };
    }
}