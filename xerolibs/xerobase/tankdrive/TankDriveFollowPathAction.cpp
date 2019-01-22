#include "TankDriveFollowPathAction.h"
#include "TankDrive.h"
#include "Robot.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {

        const std::string TankDriveFollowPathAction::action_name_("PathFollow") ;

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

            getTankDrive().getRobot().startPlot(action_name_, 13) ;
        }


        void TankDriveFollowPathAction::run() {
            auto &td = getTankDrive() ;
            auto &rb = td.getRobot() ;

            if (index_ < path_->size()) {
                auto &logger = td.getRobot().getMessageLogger() ;

                double dt = td.getRobot().getDeltaTime() ;
                const XeroSegment lseg = path_->getLeftSegment(index_) ;
                const XeroSegment rseg = path_->getRightSegment(index_) ;
                double lout = left_follower_->getOutput(lseg.getAccel(), lseg.getVelocity(), lseg.getPOS(), 
                                        left_start_ + td.getLeftDistance(), dt) ;
                double rout = right_follower_->getOutput(rseg.getAccel(), rseg.getVelocity(), rseg.getPOS(), 
                                        right_start_ + td.getRightDistance(), dt) ;

                td.setMotorsToPercents(lout, rout) ;                        


                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << td.getRobot().getTime() ;
                logger << "," << td.getRobot().getTime() - start_time_ ;
                logger << "," << lseg.getPOS() ;
                logger << "," << left_start_ + td.getLeftDistance() ;
                logger << "," << lseg.getVelocity() ;
                logger << "," << td.getLeftVelocity() ;
                logger << "," << lseg.getAccel() ;
                logger << "," << lout ;
                logger << "," << rseg.getPOS() ;
                logger << "," << right_start_ + td.getRightDistance() ;                
                logger << "," << lseg.getVelocity() ;
                logger << "," << td.getRightVelocity() ;                
                logger << "," << lseg.getAccel() ;                
                logger << "," << rout ;
                logger.endMessage() ;

                rb.addPlotData(action_name_, index_, "time", rb.getTime() - start_time_) ;

                // Left side
                rb.addPlotData(action_name_, index_, "ltpos", lseg.getPOS()) ;
                rb.addPlotData(action_name_, index_, "lapos", left_start_ + td.getLeftDistance()) ;
                rb.addPlotData(action_name_, index_, "ltvel", lseg.getVelocity()) ;
                rb.addPlotData(action_name_, index_, "lavel", td.getLeftVelocity()) ;
                rb.addPlotData(action_name_, index_, "ltaccel", lseg.getAccel()) ;
                rb.addPlotData(action_name_, index_, "lout", lout) ;

                // Right side
                rb.addPlotData(action_name_, index_, "rtpos", rseg.getPOS()) ;
                rb.addPlotData(action_name_, index_, "rapos", right_start_ + td.getRightDistance()) ;
                rb.addPlotData(action_name_, index_, "rtvel", rseg.getVelocity()) ;
                rb.addPlotData(action_name_, index_, "ravel", td.getRightVelocity()) ;
                rb.addPlotData(action_name_, index_, "rtaccel", rseg.getAccel()) ;
                rb.addPlotData(action_name_, index_, "rout", rout) ;                
            }
            else {
                if (index_ == path_->size())
                    rb.endPlot(action_name_) ;

                td.setMotorsToPercents(0.0, 0.0) ;
            }

            index_++ ;            
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