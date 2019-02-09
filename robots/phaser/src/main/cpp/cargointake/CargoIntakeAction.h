#pragma once

#include "Action.h"
#include "basegroups.h"
#include "CargoIntake.h"
#include "Robot.h"


/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class CargoIntakeAction : public Action {        
        public:
            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            CargoIntakeAction(CargoIntake &subsystem, bool is_deployed_) : subsystem_(subsystem) {
                is_done_ = false ;
                collect_power_ = subsystem.getRobot().getSettingsParser().getDouble("cargointake:collectpower") ;
            }

            ~CargoIntakeAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            
        private:
            /// The tank drive subsystem
            CargoIntake &subsystem_;
            double collect_power_ ;
            bool is_done_;
            bool is_deployed_;
        };
    }
}