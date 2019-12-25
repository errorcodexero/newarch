#include "DriveAndCollectAction.h"
#include "PhoenixRobotSubsystem.h"
#include "liftingcollector/LiftingCollector.h"
#include "liftingcollector/LiftingCollectCubeAction.h"
#include "tankdrive/TankDrive.h"
#include "tankdrive/actions/TankDriveDistanceAction.h"
#include "tankdrive/actions/TankDrivePowerAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {

        DriveAndCollectAction::DriveAndCollectAction(PhoenixRobotSubsystem &sub, double distance) : PhoenixSubsystemAction(sub) {
            distance_ = distance ;

            auto drivebase = getRobotSubsystem().getTankDrive() ;    
            stop_drivebase_ = std::make_shared<TankDrivePowerAction>(*drivebase, 0.0, 0.0) ;
        }

        DriveAndCollectAction::DriveAndCollectAction(PhoenixRobotSubsystem &sub, const std::string &distance) : PhoenixSubsystemAction(sub) {
            distance_ = sub.getRobot().getSettingsParser().getDouble(distance) ;

            auto drivebase = getRobotSubsystem().getTankDrive() ;    
            stop_drivebase_ = std::make_shared<TankDrivePowerAction>(*drivebase, 0.0, 0.0) ;            
        }

        DriveAndCollectAction::~DriveAndCollectAction() {
        }

        void DriveAndCollectAction::start() {
            state_ = State::Driving ;

            auto liftcollector = getRobotSubsystem().getLiftingCollector() ;
            auto drivebase = getRobotSubsystem().getTankDrive() ;

            start_ = drivebase->getDist() ;

            auto collect = std::make_shared<LiftingCollectCubeAction>(*liftcollector, "lifter:height:exchange") ;
            liftcollector->setAction(collect) ;

            auto drive = std::make_shared<TankDriveDistanceAction>(*drivebase, distance_) ;
            drivebase->setAction(drive) ;
        }

        bool DriveAndCollectAction::isDone() {
            return state_ == State::Collected || state_ == State::DistanceReached || state_ ==State::Cancel ;
        }

        void DriveAndCollectAction::run()  {
            auto liftcollector = getRobotSubsystem().getLiftingCollector() ;
            auto drivebase = getRobotSubsystem().getTankDrive() ;

            switch(state_) {
            case State::Driving:
                if (liftcollector->isDone()) {
                    drivebase->setAction(stop_drivebase_) ;
                    state_ = State::Stopping ;
                }
                else if (drivebase->isDone()) {
                    drivebase->setAction(stop_drivebase_) ;
                    getRobotSubsystem().getRobot().getSettingsParser().set("var:cube_collected", false) ;
                    getRobotSubsystem().getRobot().getSettingsParser().set("var:collect_distance", 0.0) ;
                    state_ = State::DistanceReached ;
                }
                break ;

            case State::Stopping:
                if (drivebase->getVelocity() < 0.1) {
                    double total = drivebase->getDist() - start_ ;
                    getRobotSubsystem().getRobot().getSettingsParser().set("var:cube_collected", true) ;
                    getRobotSubsystem().getRobot().getSettingsParser().set("var:collect_distance", total) ;                    
                    state_ = State::Collected ;
                }
                break ;

            case State::Collected:
            case State::Cancel:
            case State::DistanceReached:
                // Do nothing, just sit here
                break ;
            }
        }

        void DriveAndCollectAction::cancel() {
            state_ = State::Cancel ;
            
        }

        std::string DriveAndCollectAction::toString() {
            std::string ret = "DriveAndCollect " + std::to_string(distance_) ;
            return ret ;
        }
    }
}
