#include "TankDriveFollowPathAction.h"
#include "TankDrive.h"
#include "Robot.h"
#include <frc/smartdashboard/SmartDashboard.h>
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

            if (getTankDrive().hasGearShifter())
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
                const XeroSegment lseg = path_->getLeftSegment(index_) ;
                const XeroSegment rseg = path_->getRightSegment(index_) ;
                double lout = left_follower_->getOutput(lseg.getAccel(), lseg.getVelocity(), lseg.getPOS(), 
                                        left_start_ + getTankDrive().getLeftDistance(), dt) ;
                double rout = right_follower_->getOutput(rseg.getAccel(), rseg.getVelocity(), rseg.getPOS(), 
                                        right_start_ + getTankDrive().getRightDistance(), dt) ;

                getTankDrive().setMotorsToPercents(lout, rout) ;                        


                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << getTankDrive().getRobot().getTime() ;
                logger << "," << getTankDrive().getRobot().getTime() - start_time_ ;
                logger << "," << lseg.getPOS() ;
                logger << "," << left_start_ + getTankDrive().getLeftDistance() ;
                logger << "," << lseg.getVelocity() ;
                logger << "," << getTankDrive().getLeftVelocity() ;
                logger << "," << lseg.getAccel() ;
                logger << "," << lout ;
                logger << "," << rseg.getPOS() ;
                logger << "," << right_start_ + getTankDrive().getRightDistance() ;                
                logger << "," << lseg.getVelocity() ;
                logger << "," << getTankDrive().getRightVelocity() ;                
                logger << "," << lseg.getAccel() ;                
                logger << "," << rout ;
                logger.endMessage() ;

                frc::SmartDashboard::PutNumber("lapos", left_start_ + getTankDrive().getLeftDistance()) ;
                frc::SmartDashboard::PutNumber("ltpos", lseg.getPOS()) ;
                frc::SmartDashboard::PutNumber("lavel", getTankDrive().getLeftVelocity()) ;
                frc::SmartDashboard::PutNumber("ltvel", lseg.getVelocity()) ;

                frc::SmartDashboard::PutNumber("rapos", right_start_ + getTankDrive().getRightDistance()) ;
                frc::SmartDashboard::PutNumber("rtpos", rseg.getPOS()) ;
                frc::SmartDashboard::PutNumber("ravel", getTankDrive().getRightVelocity()) ;
                frc::SmartDashboard::PutNumber("rtvel", rseg.getVelocity()) ;                

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