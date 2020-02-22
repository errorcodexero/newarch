#include "ExtendClimberAction.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveDistanceAction.h>
#include <tankdrive/actions/TankDriveTimedPowerAction.h>
#include "cargointake/CargoIntake.h"
#include "cargointake/CargoIntakeAction.h"
#include "climber/Climber.h"
#include "climber/ClimberDeployAction.h"
#include <iostream>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ExtendClimberAction::ExtendClimberAction(PhaserRobotSubsystem &subsystem, bool complete): subsystem_(subsystem)
        {
            auto cargo_intake = subsystem_.getGameManipulator()->getCargoIntake() ;
            auto db = subsystem_.getTankDrive() ;
            auto climber = subsystem_.getClimber() ;

            complete_ = complete ;

            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            extend_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, true) ;
            deploy_grasshopper_ = std::make_shared<ClimberDeployAction>(*climber) ;            
            drive_back_ = std::make_shared<TankDriveTimedPowerAction>(*db, "climb:backup:left",
                            "climb:backup:right", "climb:backup:duration", true) ;
            drivebase_power_ = std::make_shared<TankDriveTimedPowerAction>(*db, "climb:drive:left", 
                            "climb:drive:right", "climb:drive:duration", true) ;
                            
            deploy_delay_ = subsystem_.getRobot().getSettingsParser().getDouble("climb:deploy_delay") ;
            backup_delay_ = subsystem_.getRobot().getSettingsParser().getDouble("climb:backup_delay") ;
            
            velocity_sample_count_ = subsystem_.getRobot().getSettingsParser().getInteger("climb:velocity_samples") ;
            velocity_still_threshold_ = subsystem_.getRobot().getSettingsParser().getDouble("climb:still_threshold") ;

            state_ = State::Idle ;            
        }


        ExtendClimberAction::~ExtendClimberAction() {
        }

        void ExtendClimberAction::start() {
            auto climber = subsystem_.getClimber() ;
            climber->setAction(deploy_grasshopper_) ;

            state_ = State::ReleaseGrasshopper ;
        }

        void ExtendClimberAction::run() {
            auto db = subsystem_.getTankDrive() ;

            switch(state_) {
            case State::ReleaseGrasshopper:
                if (deploy_grasshopper_->isDone()) {
                    start_ = subsystem_.getRobot().getTime() ;
                    state_ = State::WaitForDeploy ;
                }
                break ;

            case State::WaitForDeploy:
                if (subsystem_.getRobot().getTime() - start_ > deploy_delay_) {
                    auto cargo = subsystem_.getGameManipulator()->getCargoIntake() ;
                    cargo->setAction(extend_cargo_intake_) ;
     
                    auto db = subsystem_.getTankDrive() ;
                    db->setAction(drivebase_power_) ;   

                    state_ = State::StartWheels ;   
                }
                break ;

            case State::DeployCargoCollector:
                break ;               

            case State::StartWheels:
                if (drivebase_power_->isDone()) {
                    start_ = subsystem_.getRobot().getTime() ;
                    state_ = State::WaitForStopped ;
                }
                break ;

            case State::WaitForStopped:
                if (subsystem_.getRobot().getTime() - start_ > backup_delay_) {
                    db->setAction(drive_back_) ;                    
                    state_ = State::Backup ;
                }
                break ;

            case State::Backup:
                std::cout << "Backup" << std::endl ;               
                if (drive_back_->isDone()) {
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }
        
        bool ExtendClimberAction::isDone() {
            return state_ == State::Idle ;
        }

        void ExtendClimberAction::abort() {
            state_ = State::Idle ;
        }

        void ExtendClimberAction::cancel() {
        }

        std::string ExtendClimberAction::toString() {
            std::string ret("ExtendClimberAction") ;
            return ret ;
        }
    }
}