#pragma once

#include "phaseroi/PhaserOISubsystem.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "hatchholder/HatchHolder.h"
#include "cargoholder/CargoHolder.h"
#include "cargointake/CargoIntake.h"
#include "turntable/TurnTable.h"
#include "phasercameratracker/PhaserCameraTracker.h"
#include "climber/Climber.h"
#include <tankdrive/TankDrive.h>
#include <lifter/Lifter.h>
#include <cameratracker/CameraTracker.h>
#include <RobotSubsystem.h>

namespace xero {
    namespace phaser {

        //
        // Add gamemanipulator
        // Add climber
        //
        class PhaserRobotSubsystem : public xero::base::RobotSubsystem {
        public:
            PhaserRobotSubsystem(xero::base::Robot &robot) ;
            virtual ~PhaserRobotSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<PhaserOISubsystem> getOI() {
                return oi_ ;
            }    

            std::shared_ptr<xero::base::LightSensorSubsystem> getLightSensor() {
                return light_sensor_ ;
            }

            std::shared_ptr<HatchHolder> getHatchHolder() {
                return hatch_holder_ ;
            }            

            std::shared_ptr<xero::base::Lifter> getLifter() {
                return lifter_ ;
            }

            std::shared_ptr<PhaserCameraTracker> getCameraTracker() {
                return tracker_ ;
            }            

            std::shared_ptr<Turntable> getTurntable() {
                return turntable_ ;
            }

            std::shared_ptr<CargoHolder> getCargoHolder() {
                return cargo_holder_ ;
            }

            std::shared_ptr<CargoIntake> getCargoIntake() {
                return cargo_intake_ ;
            }

            std::shared_ptr<Climber> getClimber() {
                return climber_ ;
            }

        private:
            std::shared_ptr<PhaserOISubsystem> oi_ ;
            std::shared_ptr<xero::base::LightSensorSubsystem> light_sensor_ ;
            std::shared_ptr<HatchHolder> hatch_holder_ ;
            std::shared_ptr<CargoHolder> cargo_holder_ ;
            std::shared_ptr<CargoIntake> cargo_intake_ ;
            std::shared_ptr<xero::base::Lifter> lifter_ ;
            std::shared_ptr<PhaserCameraTracker> tracker_ ;
            std::shared_ptr<Turntable> turntable_ ;
            std::shared_ptr<Climber> climber_ ;
        } ;
    }
}