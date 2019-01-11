#include "TankDriveFollowPathAction.h"
#include "TankDrive.h"
#include "Robot.h"
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {
        TankDriveFollowPathAction::TankDriveFollowPathAction(TankDrive &db, const std::string &name) : TankDriveAction(db)  {
            path_ = db.getRobot().getPathManager()->getPath(name) ;
            assert(path_ != nullptr) ;
            //vapd
            left_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), "tankdrive:follower:left:kv", 
                                "tankdrive:follower:left:ka", "tankdrive:follower:left:kp", "tankdrive:follower:left:kd") ;
            right_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), "tankdrive:follower:right:kv", 
                                "tankdrive:follower:right:ka", "tankdrive:follower:right:kp", "tankdrive:follower:right:kd") ;                                
        }

        TankDriveFollowPathAction::~TankDriveFollowPathAction() {                
        }

        void TankDriveFollowPathAction::start() {
            xstart_ = (path_->getLeft(0).getX() + path_->getRight(0).getX()) / 2.0 ;
            ystart_ = (path_->getLeft(0).getY() + path_->getRight(0).getY()) / 2.0 ;
            index_ = 0 ;            
        }


        void TankDriveFollowPathAction::run() {
            index_++ ;

            if (index_ < path_->size()) {
                double dt = getTankDrive().getRobot().getDeltaTime() ;
                const XeroPathSegment &lseg = path_->getLeft(index_) ;
                const XeroPathSegment &rseg = path_->getRight(index_) ;
                double lout = left_follower_->getOutput(lseg.getAcceleration(), lseg.getVelocity(), lseg.getPOS(), 
                                        getTankDrive().getLeftDistance(), dt) ;
                double rout = right_follower_->getOutput(rseg.getAcceleration(), rseg.getVelocity(), rseg.getPOS(), 
                                        getTankDrive().getRightDistance(), dt) ;

                getTankDrive().setMotorsToPercents(lout, rout) ;                        
            }
            else {
                getTankDrive().setMotorsToPercents(0.0, 0.0) ;
            }
        }

        bool TankDriveFollowPathAction::isDone() {
            return index_ >= path_->size() ;
        }

        void TankDriveFollowPathAction::cancel()  {
            index_ = path_->size() ;
            getTankDrive().setMotorsToPercents(0.0, 0.0) ;            
        }

        std::string TankDriveFollowPathAction::toString() {
            return "TankDriveFollowPathAction " + path_->getName() ;
        }
    }
}