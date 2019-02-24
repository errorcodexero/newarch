#pragma once

#include "Action.h"
#include "basegroups.h"
#include "HatchHolder.h"



/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class HatchHolderAction : public Action {
        
        public:

            enum class Operation {
                EXTEND_ARM,
                RETRACT_ARM,
                EXTEND_FINGER,
                RETRACT_FINGER
            };

            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            HatchHolderAction(HatchHolder &subsystem, Operation operation, const std::string &name) : subsystem_(subsystem), operation_(operation) {
                is_done_ = false ;
                duration_ = subsystem.getRobot().getSettingsParser().getDouble(name) ;
            }

            ~HatchHolderAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            virtual std::string toString(Operation oper) ;
            
        private:
            /// The tank drive subsystem
            HatchHolder &subsystem_;
            Operation operation_;
            bool is_done_;
            double start_ ;
            double duration_ ;
        };
    }
}
