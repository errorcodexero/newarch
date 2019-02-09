#pragma once

#include "Action.h"
#include "basegroups.h"
#include "CargoHolder.h"



/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class CargoHolderAction : public Action {
        
        public:

            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            CargoHolderAction(CargoHolder &subsystem, double power) : subsystem_(subsystem) {
                is_done_ = false ;
                power_ = power ; 
            }

            ~CargoHolderAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            
        private:
            /// The tank drive subsystem
            CargoHolder &subsystem_;
            double power_ ;
            bool is_done_;
        };
    }
}
