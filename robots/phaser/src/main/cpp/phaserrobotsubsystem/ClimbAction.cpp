#include "ClimbAction.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDriveTimedPowerAction.h>
#include "cargointake/CargoIntake.h"
#include "cargointake/CargoIntakeAction.h"
#include "climber/Climber.h"
#include "climber/ClimberDeployAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ClimbAction::ClimbAction(PhaserRobotSubsystem &subsystem): subsystem_(subsystem)
        {
            auto cargo_intake = subsystem_.getGameManipulator()->getCargoIntake() ;
            auto db = subsystem_.getTankDrive() ;
            auto climber = subsystem_.getClimber() ;

            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            extend_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, true) ;
            drive_back_ = std::make_shared<TankDriveDistanceAction>(*db, "climb:backup_distance") ;
            deploy_grasshopper_ = std::make_shared<ClimberDeployAction>(*climber) ;
            drivebase_power_ = std::make_shared<TankDriveTimedPowerAction>(*db, "climb:drive:left", 
                            "climb:drive:right", "climb:drive:duration", true) ;

            delay_duration_ = subsystem_.getRobot().getSettingsParser().getDouble("climb:deploy_delay") ;
            
            state_ = State::Idle ;
        }


        ClimbAction::~ClimbAction() {
        }

        void ClimbAction::start() {
            auto cargo_intake = subsystem_.getGameManipulator()->getCargoIntake() ;
            cargo_intake->setAction(retract_cargo_intake_) ;

            state_ = State::RetractCargoIntake ;
        }

        void ClimbAction::run() {
            switch(state_) {
            case State::RetractCargoIntake:
                if (retract_cargo_intake_->isDone()) {
                    auto db = subsystem_.getTankDrive() ;
                    db->setAction(drive_back_) ;
                    state_ = State::BackupDrivebase ;
                }
                break ;

            case State::BackupDrivebase:
                if (drive_back_->isDone()) {
                    auto climber = subsystem_.getClimber() ;
                    climber->setAction(deploy_grasshopper_) ;

                    auto cargo_intake = subsystem_.getGameManipulator()->getCargoIntake() ;
                    cargo_intake->setAction(extend_cargo_intake_) ;

                    state_ = State::ReleaseGrasshopper ;
                }
                break ;

            case State::ReleaseGrasshopper:
                if (deploy_grasshopper_->isDone()) {
                    delay_start_ = subsystem_.getRobot().getTime() ;
                    state_ = State::WaitForDeploy ;
                }
                break ;

            case State::WaitForDeploy:
                if (subsystem_.getRobot().getTime() - delay_start_ > delay_duration_) {
                    auto db = subsystem_.getTankDrive() ;
                    db->setAction(drivebase_power_) ;
                    state_ = State::StartWheels ;                    
                }
                break ;

            case State::StartWheels:
                if (drivebase_power_->isDone()) {
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }
        
        bool ClimbAction::isDone() {
            return state_ == State::Idle ;
        }

        void ClimbAction::abort() {
            state_ = State::Idle ;
        }

        void ClimbAction::cancel() {
        }

        std::string ClimbAction::toString() {
            std::string ret("ClimbAction") ;
            return ret ;
        }
    }
}