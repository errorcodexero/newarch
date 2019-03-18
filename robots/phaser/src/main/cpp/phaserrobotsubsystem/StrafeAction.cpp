#include "StrafeAction.h"
#include "gamepiecemanipulator/ScoreCargo.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDrivePowerAction.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        StrafeAction::StrafeAction(PhaserRobotSubsystem &subsystem, bool reverse, int count): subsystem_(subsystem)
        {
            auto db = subsystem_.getTankDrive() ;
            auto game = subsystem_.getGameManipulator() ;

            count_ = count ;
            state_ = State::Idle ;
            reverse_ = reverse ;

            double dist = subsystem.getRobot().getSettingsParser().getDouble("strafe:drive_distance") ;
            double velocity = subsystem.getRobot().getSettingsParser().getDouble("strafe:maxv") ;
            shoot_dist_ = subsystem.getRobot().getSettingsParser().getDouble("strafe:shoot_distance") ;

            if (reverse_)
            {
                dist = -dist ;
                velocity = -velocity ;
            }

            drive_straight_ = std::make_shared<TankDriveDistanceAction>(*db, dist, velocity) ;
            drive_stop_ = std::make_shared<TankDrivePowerAction>(*db, 0.0, 0.0, true) ;
            shoot_ = std::make_shared<ScoreCargo>(*game) ;
        }

        StrafeAction::~StrafeAction() {
        }

        void StrafeAction::start() {
            state_ = State::SearchingForLine ;
            auto db = subsystem_.getTankDrive() ;
            db->setAction(drive_straight_) ;
            detected_ = false ;
            thiscount = count_ ;
        }

        void StrafeAction::run() {
            auto ls = (reverse_ ? subsystem_.getBackLineSensor() : subsystem_.getFrontLineSensor()) ;
            auto db = subsystem_.getTankDrive() ;
            auto game = subsystem_.getGameManipulator() ;

            switch(state_) {
            case State::SearchingForLine:
                if (detected_ && !ls->detectedObject()) {
                    thiscount-- ;
                    detected_ = false ;
                    if (thiscount == 0) {
                        state_ = State::ToShootPosition ;
                        trigger_dist_ = db->getDist() ;
                    }
                }
                else if (ls->detectedObject()) {
                    detected_ = true ;
                }
                break ;

            case State::ToShootPosition:
                if (std::fabs(db->getDist() - trigger_dist_) > shoot_dist_) {
                    state_ = State::Shoot ;
                    game->setAction(shoot_) ;
                }
                break ;

            case State::Shoot:
                if (shoot_->isDone()) {
                    state_ = State::Idle ;
                    db->setAction(drive_stop_) ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }
        
        bool StrafeAction::isDone() {
            return state_ == State::Idle ;
        }

        void StrafeAction::abort() {
            state_ = State::Idle ;
        }

        void StrafeAction::cancel() {
        }

        std::string StrafeAction::toString() {
            std::string ret("StrafeAction") ;
            return ret ;
        }
    }
}