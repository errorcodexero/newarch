#pragma once

#include "Action.h"
#include "basegroups.h"
#include "SingleMotorSubsystem.h"
#include "SingleMotorSubsystemAction.h"

namespace xero {
    namespace base {
        class SingleMotorVoltageAction : public SingleMotorSubsystemAction {
        public:

            SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, double duty) ;

            SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name) ;

            SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, double duty, double duration) ;

            SingleMotorVoltageAction(SingleMotorSubsystem &single_motor_subsystem, const std::string &name, const std::string &durname) ;			

            virtual ~SingleMotorVoltageAction() ;

            virtual void start() ;
        
            virtual void run() ;
 
            virtual bool isDone() ;

            virtual void cancel() ;

            virtual std::string toString() ;
            
        private:
            double duty_cycle_;
			bool timed_ ;
			double duration_ ;
			bool is_done_ ;
			double start_ ;
        };
    }
}