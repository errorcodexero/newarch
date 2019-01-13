#pragma once
#include <Subsystem.h>
#include <frc/DigitalInput.h>
#include <memory>
#include <cstdint>

namespace xero {
    namespace phaser {
        /// \brief the grabber subsystem that opens and closes the intake arms.
        /// This subsystem controls a single motor that controls the angle of the
        /// grabber arms.  There is an encoder on the motor that is used to detect
        /// athe angle of the grabber arms.
        class LineFollower : public xero::base::Subsystem {
        public:
            LineFollower(xero::base::Robot &robot, uint64_t mid) ;
            virtual ~LineFollower() ;

            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;

        private:
            std::shared_ptr<frc::DigitalInput> left_ ;
            std::shared_ptr<frc::DigitalInput> right_ ;
            std::shared_ptr<frc::DigitalInput> middle_ ;    

            bool left_state_ ;
            bool right_state_ ;
            bool middle_state_ ;      

            uint64_t msg_id_ ;              
        } ;
    }
}
