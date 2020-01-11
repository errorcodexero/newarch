#pragma once

#include "motorcontrolleroi/MotorControllerOISubsystem.h"
#include "testsubsystem/TestSubsystem.h"
#include "colorsensor/ColorSensorSubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace motorcontroller {

        //
        // Add gamemanipulator
        // Add climber
        //
        class MotorControllerSubsystem : public xero::base::RobotSubsystem {
        public:
            MotorControllerSubsystem(xero::base::Robot &robot) ;
            virtual ~MotorControllerSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<MotorControllerOISubsystem> getOI() {
                return oi_ ;
            }    

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<MotorControllerOISubsystem> oi_ ;
            std::shared_ptr<TestSubsystem> test_;
            std::shared_ptr<ColorSensorSubsystem> sensor_ ;
        } ;
    }
}
