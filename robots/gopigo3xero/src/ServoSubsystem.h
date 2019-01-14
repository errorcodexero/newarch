#pragma once

#include <frc/Servo.h>
#include <Subsystem.h>

namespace xero {
    namespace gopigo {
        class ServoSubsystem : public xero::base::Subsystem {
            friend class ServoGoToAngle ;
            
        public:
            ServoSubsystem(xero::base::Robot &robot) ;

            virtual ~ServoSubsystem() ;

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<frc::Servo> servo_ ;
        } ;
    }
}
