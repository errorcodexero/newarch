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
        ClimbAction::ClimbAction(PhaserRobotSubsystem &subsystem, bool complete): subsystem_(subsystem)
        {
            auto cargo_intake = subsystem_.getGameManipulator()->getCargoIntake() ;
            auto db = subsystem_.getTankDrive() ;
            auto climber = subsystem_.getClimber() ;

            complete_ = complete ;

            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            extend_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, true) ;
            drive_back_ = std::make_shared<TankDriveTimedPowerAction>(*db, "climb:backup:left",
                            "climb:backup:right", "climb:backup:duration", true) ;
            deploy_grasshopper_ = std::make_shared<ClimberDeployAction>(*climber) ;
            drivebase_power_ = std::make_shared<TankDriveTimedPowerAction>(*db, "climb:drive:left", 
                            "climb:drive:right", "climb:drive:duration", true) ;

            delay_duration_ = subsystem_.getRobot().getSettingsParser().getDouble("climb:deploy_delay") ;
            
            state_ = State::Idle ;
            velocity_sample_count_ = subsystem_.getRobot().getSettingsParser().getInteger("climb:velocity_samples") ;
            velocity_still_threshold_ = subsystem_.getRobot().getSettingsParser().getDouble("climb:still_threshold") ;
        }


        ClimbAction::~ClimbAction() {
        }

        void ClimbAction::start() {
            auto climber = subsystem_.getClimber() ;
            climber->setAction(deploy_grasshopper_) ;

            state_ = State::ReleaseGrasshopper ;
        }

        void ClimbAction::run() {
            auto db = subsystem_.getTankDrive() ;

            switch(state_) {
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
                    auto cargo = subsystem_.getGameManipulator()->getCargoIntake() ;
                    cargo->setAction(extend_cargo_intake_) ;
                    state_ = State::DeployCargoCollector ;
                }
                break ;

            case State::DeployCargoCollector:
                if (extend_cargo_intake_->isDone()) {
                    if (!complete_)
                        state_ = State::Idle ;
                    else
                        state_ = State::WaitForStopped ;
                }
                break ;

            case State::WaitForStopped:
                velocities_.push_back(db->getXYZVelocity()) ;
                if (velocities_.size() > velocity_sample_count_)
                    velocities_.pop_back() ;

                if (velocities_.size() == velocity_sample_count_) {
                    double sum = 0.0 ;
                    for(double d : velocities_)
                        sum += d ;

                    sum /= static_cast<double>(velocities_.size()) ;
                    if (sum < velocity_still_threshold_) {
                        state_ = State::Backup ;
                        db->setAction(drive_back_) ;
                    }
                }
                break ;

            case State::Backup:
                if (drive_back_->isDone()) {
                    state_ = State::Idle ;
                }

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