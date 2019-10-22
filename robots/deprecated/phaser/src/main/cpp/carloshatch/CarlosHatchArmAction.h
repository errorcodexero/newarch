#pragma once

#include "CarlosHatchAction.h"
#include "basegroups.h"
#include "CarlosHatch.h"



/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class CarlosHatchArmAction : public CarlosHatchAction {
        
        public:
            enum class Operation {
                EXTEND,
                RETRACT,
                OPEN,
                CLOSE
            };

            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            CarlosHatchArmAction(CarlosHatch &subsystem, Operation operation) : CarlosHatchAction(subsystem), operation_(operation) {
                is_done_ = false ;
            }

            virtual ~CarlosHatchArmAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            virtual std::string toString(Operation oper) ;

        private:
            constexpr static const double epsilon_ = 0.001 ;
            
        private:
            Operation operation_;
            bool is_done_;
        };
    }
}
