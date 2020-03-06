#pragma once

#include "droidoi/DroidOISubsystem.h"
#include <tankdrive/TankDrive.h>
#include <RobotSubsystem.h>
#include <AlarmSubsystem.h>

namespace xero {
    namespace droid {
        class ControlPanelRotator;
        class Climber;
        class GamePieceManipulator;
        class Turret;
        class DroidLimeLight;
        class TargetTracker;

        //
        // Add gamemanipulator
        // Add climber
        //
        class DroidSubsystem : public xero::base::RobotSubsystem {
        public:
            DroidSubsystem(xero::base::Robot &robot) ;
            virtual ~DroidSubsystem() ;

            std::shared_ptr<xero::base::TankDrive> getTankDrive() {
                return std::dynamic_pointer_cast<xero::base::TankDrive>(getDriveBase()) ;
            }
          
            std::shared_ptr<DroidOISubsystem> getOI() {
                return oi_ ;
            }
          
            std::shared_ptr<ControlPanelRotator> getControlPanelRotator() {
                return control_panel_rotator_ ;
            }    

            std::shared_ptr<GamePieceManipulator> getGamePieceManipulator() {
                return manip_;
            }

            std::shared_ptr<Turret> getTurret() {
                return turret_;
            }

            std::shared_ptr<DroidLimeLight> getLimeLight() {
                return limelight_;
            }

            std::shared_ptr<TargetTracker> getTargetTracker() {
                return target_tracker_;
            }

            std::shared_ptr<Climber> getClimber() {
                return climber_ ;
            }

            std::shared_ptr<xero::base::AlarmSubsystem> getAlarms() {
                return alarms_ ;
            }

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;
            
        private:
            std::shared_ptr<DroidOISubsystem> oi_ ;
            std::shared_ptr<ControlPanelRotator> control_panel_rotator_;
            std::shared_ptr<Climber> climber_;
            std::shared_ptr<GamePieceManipulator> manip_;
            std::shared_ptr<Turret> turret_;
            std::shared_ptr<DroidLimeLight> limelight_;
            std::shared_ptr<TargetTracker> target_tracker_;
            std::shared_ptr<xero::base::AlarmSubsystem> alarms_ ;
        } ;
    }
}