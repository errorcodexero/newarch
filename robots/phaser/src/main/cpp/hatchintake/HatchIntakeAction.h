#pragma once

#include "Action.h"
#include "basegroups.h"
#include "HatchIntake.h"
#include "Robot.h"


/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class HatchIntakeAction : public Action {        
        public:
            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            HatchIntakeAction(HatchIntake &subsystem, bool deployed) : subsystem_(subsystem) {
                is_done_ = false ;
                collect_power_ = subsystem.getRobot().getSettingsParser().getDouble("hatchintake:power") ;
                is_deployed_ = deployed ;
            }

            ~HatchIntakeAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            
        private:
            /// The tank drive subsystem
            HatchIntake &subsystem_;
            double collect_power_ ;
            bool is_done_;
            bool is_deployed_;
        };
    }
}