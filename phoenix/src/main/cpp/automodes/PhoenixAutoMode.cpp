#include "PhoenixAutoMode.h"
#include "phoenixsubsystem/PhoenixRobotSubsystem.h"
#include "liftingcollector/LiftingCollector.h"
#include "tankdrive/TankDriveDistanceAction.h"
#include "tankdrive/TankDriveAngleAction.h"
#include "phlifter/LifterGoToHeightAction.h"
#include "phlifter/LifterCalibrateAction.h"
#include "collector/CollectorEjectCubeAction.h"
#include "grabber/GrabberCalibrateAction.h"
#include "phoenixsubsystem/DriveAndCollectAction.h"

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        PhoenixAutoMode::PhoenixAutoMode(Phoenix &robot, const std::string &name) : ActionSequence(robot.getMessageLogger(), name), robot_(robot) {
        }
        
        PhoenixAutoMode::~PhoenixAutoMode() {
        }

        void PhoenixAutoMode::addAction(xero::base::SubsystemPtr sub, xero::base::ActionPtr act, bool block) {
            if (stack_.size() == 0)
                pushSubActionPair(sub, act, block) ;
            else
                stack_.front()->pushSubActionPair(sub, act, block) ;
        }

        void PhoenixAutoMode::addAction(xero::base::ActionPtr act) {
            if (stack_.size() == 0)
                pushAction(act) ;
            else
                stack_.front()->pushAction(act) ;
        }

        void PhoenixAutoMode::calibrate() {
            ActionPtr action ;
            auto grabbersub = robot_.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector()->getGrabber() ;
            auto liftersub = robot_.getPhoenixRobotSubsystem()->getLiftingCollector()->getLifter() ;

            action = std::make_shared<LifterCalibrateAction>(*liftersub) ;
            addAction(liftersub, action) ;

            action = std::make_shared<GrabberCalibrateAction>(*grabbersub) ;
            addAction(grabbersub, action) ;
        }

        void PhoenixAutoMode::driveStraight(const std::string &name) {
            auto db = robot_.getPhoenixRobotSubsystem()->getTankDrive() ;
            auto act = std::make_shared<TankDriveDistanceAction>(*db, name) ;
            addAction(db, act) ;
        }

        void PhoenixAutoMode::rotate(const std::string &name) {
            auto db = robot_.getPhoenixRobotSubsystem()->getTankDrive() ;
            auto act = std::make_shared<TankDriveAngleAction>(*db, name) ;
            addAction(db, act) ;
        }   

        void PhoenixAutoMode::rotate(double angle) {
            auto db = robot_.getPhoenixRobotSubsystem()->getTankDrive() ;
            auto act = std::make_shared<TankDriveAngleAction>(*db, angle) ;
            addAction(db, act) ;
        }               

        void PhoenixAutoMode::lift(const std::string &name, bool block) {
            auto liftersub = robot_.getPhoenixRobotSubsystem()->getLiftingCollector()->getLifter() ;

            auto act = std::make_shared<LifterGoToHeightAction>(*liftersub, name) ;
            addAction(liftersub, act, block) ;
        }

        void PhoenixAutoMode::eject(const std::string &name) {
            auto collectorsub = robot_.getPhoenixRobotSubsystem()->getLiftingCollector()->getCollector() ;

            auto act = std::make_shared<CollectorEjectCubeAction>(*collectorsub, name) ;
            addAction(collectorsub, act) ;
        }

        void PhoenixAutoMode::driveAndCollect(const std::string &name) {
            auto phoenixsub = robot_.getPhoenixRobotSubsystem() ;
            auto act = std::make_shared<DriveAndCollectAction>(*phoenixsub, name) ;
            addAction(phoenixsub, act) ;
        }
    }
}
