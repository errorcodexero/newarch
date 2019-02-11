#pragma once

#include "phaseroi/PhaserOISubsystem.h"
#include "lightsensor/LightSensorSubsystem.h"
#include "phasercameratracker/PhaserCameraTracker.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "climber/Climber.h"
#include <tankdrive/TankDrive.h>
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

            std::shared_ptr<PhaserCameraTracker> getCameraTracker() {
                return tracker_ ;
            }            

            std::shared_ptr<Climber> getClimber() {
                return climber_ ;
            }

            std::shared_ptr<GamePieceManipulator> getGameManipulator() {
                return game_piece_man_ ;
            }

        private:
            std::shared_ptr<PhaserOISubsystem> oi_ ;
            std::shared_ptr<xero::base::LightSensorSubsystem> light_sensor_ ;
            std::shared_ptr<PhaserCameraTracker> tracker_ ;
            std::shared_ptr<GamePieceManipulator> game_piece_man_ ;
            std::shared_ptr<Climber> climber_ ;
        } ;
    }
}