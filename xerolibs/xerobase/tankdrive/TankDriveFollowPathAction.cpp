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
            
            left_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), "tankdrive:follower:left:kv", 
                                "tankdrive:follower:left:ka", "tankdrive:follower:left:kp", "tankdrive:follower:left:kd") ;
            right_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), "tankdrive:follower:right:kv", 
                                "tankdrive:follower:right:ka", "tankdrive:follower:right:kp", "tankdrive:follower:right:kd") ;                                
        }

        TankDriveFollowPathAction::~TankDriveFollowPathAction() {                
        }

        void TankDriveFollowPathAction::start() {
            left_start_ = path_->getLeftStartPos() ;
            right_start_ = path_->getRightStartPos() ;
            index_ = 0 ;         
            start_time_ = getTankDrive().getRobot().getTime() ;

            getTankDrive().highGear() ;

            auto &logger = getTankDrive().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
            logger << "time" ;
            logger << ",runtime" ;
            logger << ",ltpos" ;
            logger << ",lapos" ;
            logger << ",ltvel" ;
            logger << ",lavel";
            logger << ",ltaccel" ;
            logger << ",lout" ;
            logger << ",rtpos" ;
            logger << ",rapos" ;
            logger << ",rtvel" ;
            logger << ",ravel" ;
            logger << ",rtaccel" ;
            logger << ",rout" ;
            logger.endMessage() ;            
        }


        void TankDriveFollowPathAction::run() {
            if (index_ < path_->size()) {
                auto &logger = getTankDrive().getRobot().getMessageLogger() ;

                double dt = getTankDrive().getRobot().getDeltaTime() ;
                const XeroPathSegment lseg = path_->getLeft(index_) ;
                const XeroPathSegment rseg = path_->getRight(index_) ;
                double lout = left_follower_->getOutput(lseg.getAcceleration(), lseg.getVelocity(), lseg.getPOS(), 
                                        left_start_ + getTankDrive().getLeftDistance(), dt) ;
                double rout = right_follower_->getOutput(rseg.getAcceleration(), rseg.getVelocity(), rseg.getPOS(), 
                                        right_start_ + getTankDrive().getRightDistance(), dt) ;

                getTankDrive().setMotorsToPercents(lout, rout) ;                        


                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << getTankDrive().getRobot().getTime() ;
                logger << "," << getTankDrive().getRobot().getTime() - start_time_ ;
                logger << "," << lseg.getPOS() ;
                logger << "," << left_start_ + getTankDrive().getLeftDistance() ;
                logger << "," << lseg.getVelocity() ;
                logger << "," << getTankDrive().getLeftVelocity() ;
                logger << "," << lseg.getAcceleration() ;
                logger << "," << lout ;
                logger << "," << rseg.getPOS() ;
                logger << "," << right_start_ + getTankDrive().getRightDistance() ;                
                logger << "," << lseg.getVelocity() ;
                logger << "," << getTankDrive().getRightVelocity() ;                
                logger << "," << lseg.getAcceleration() ;                
                logger << "," << rout ;
                logger.endMessage() ;

                index_++ ;
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