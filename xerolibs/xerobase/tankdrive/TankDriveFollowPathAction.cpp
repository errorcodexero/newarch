#include "TankDriveFollowPathAction.h"
#include "TankDrive.h"
#include "Robot.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::list<std::string> TankDriveFollowPathAction::plot_columns_ = {
            "time", 
            "ltpos", "lapos", "ltvel", "lavel", "ltaccel", "lout",
            "rtpos", "rapos", "rtvel", "ravel", "rtaccel", "rout",
            "xa", "ya", "xt", "yt"
        } ;

        TankDriveFollowPathAction::TankDriveFollowPathAction(TankDrive &db, const std::string &name, const std::string &follow, bool reverse) : TankDriveAction(db)  {
            reverse_ = reverse;
            path_ = db.getRobot().getPathManager()->getPath(name) ;
            assert(path_ != nullptr) ;

            std::string name = "tankdrive:follower:" + follow + ":";
            
            left_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), name + "left:kv", 
                                name + "left:ka", name + "left:kp", name + "left:kd") ;
            right_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), name + "right:kv", 
                                name + "right:ka", name + "right:kp", name + "right:kd") ;

            turn_correction_ = db.getRobot().getSettingsParser().getDouble("tankdrive:follower:turn_correction") ;
            angle_decay_ = db.getRobot().getSettingsParser().getDouble("tankdrive:follower:angle_decay") ;
        }

        TankDriveFollowPathAction::~TankDriveFollowPathAction() {                
        }

        void TankDriveFollowPathAction::start() {
            if (reverse_) {
                left_start_ = -path_->getLeftStartPos() ;
                right_start_ = -path_->getRightStartPos() ;
            }
            else {
                left_start_ = path_->getLeftStartPos() ;
                right_start_ = path_->getRightStartPos() ;                
            }
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
            logger << ",lticks" ;
            logger << ",rtpos" ;
            logger << ",rapos" ;
            logger << ",rtvel" ;
            logger << ",ravel" ;
            logger << ",rtaccel" ;
            logger << ",rout" ;
            logger << ",rticks" ;
            logger.endMessage() ;

            plotid_ = getTankDrive().getRobot().startPlot(toString(), plot_columns_) ;
        }


        void TankDriveFollowPathAction::run() {
            auto &td = getTankDrive() ;
            auto &rb = td.getRobot() ;

            if (index_ < path_->size()) {
                auto &logger = td.getRobot().getMessageLogger() ;

                double dt = td.getRobot().getDeltaTime() ;
                const XeroSegment lseg = path_->getLeftSegment(index_) ;
                const XeroSegment rseg = path_->getRightSegment(index_) ;

                double laccel, lvel, lpos ;
                double raccel, rvel, rpos ;

                if (reverse_) {
                    laccel = -lseg.getAccel() ;
                    lvel = -lseg.getVelocity() ;
                    lpos = -lseg.getPOS() ;
                    raccel = -rseg.getAccel() ;
                    rvel = -rseg.getVelocity() ;
                    rpos = -rseg.getPOS() ;
                }
                else {
                    laccel = lseg.getAccel() ;
                    lvel = lseg.getVelocity() ;
                    lpos = lseg.getPOS() ;
                    raccel = rseg.getAccel() ;
                    rvel = rseg.getVelocity() ;
                    rpos = rseg.getPOS() ;
                }

                double lout = left_follower_->getOutput(laccel, lvel, lpos, left_start_ + td.getLeftDistance(), dt) ;
                double rout = right_follower_->getOutput(raccel, rvel, rpos, right_start_ + td.getRightDistance(), dt) ;

                double dv = lseg.getVelocity() - rseg.getVelocity() ;
                double correct = dv * turn_correction_ ;
                lout += correct ;
                rout -= correct ;

                double angerr = lseg.getHeading() - td.getAngle() ;
                angle_error_ = angle_error_ * angle_decay_ + angerr ;

                setMotorsToPercents(lout, rout) ;

                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << td.getRobot().getTime() ;
                logger << "," << td.getRobot().getTime() - start_time_ ;
                logger << "," << lseg.getPOS() ;
                logger << "," << left_start_ + td.getLeftDistance() ;
                logger << "," << lseg.getVelocity() ;
                logger << "," << td.getLeftVelocity() ;
                logger << "," << lseg.getAccel() ;
                logger << "," << lout ;
                logger << "," << td.getLeftTickCount() ;
                logger << "," << rseg.getPOS() ;
                logger << "," << right_start_ + td.getRightDistance() ;                
                logger << "," << lseg.getVelocity() ;
                logger << "," << td.getRightVelocity() ;                
                logger << "," << lseg.getAccel() ;                
                logger << "," << rout ;
                logger << "," << td.getRightTickCount() ;
                logger.endMessage() ;

                rb.addPlotData(plotid_, index_, 0, rb.getTime() - start_time_) ;

                // Left side
                rb.addPlotData(plotid_, index_, 1, lpos) ;
                rb.addPlotData(plotid_, index_, 2, left_start_ + td.getLeftDistance()) ;
                rb.addPlotData(plotid_, index_, 3, lvel) ;
                rb.addPlotData(plotid_, index_, 4, td.getLeftVelocity()) ;
                rb.addPlotData(plotid_, index_, 5, laccel) ;
                rb.addPlotData(plotid_, index_, 6, lout) ;

                // Right side
                rb.addPlotData(plotid_, index_, 7, rpos) ;
                rb.addPlotData(plotid_, index_, 8, right_start_ + td.getRightDistance()) ;
                rb.addPlotData(plotid_, index_, 9, rvel) ;
                rb.addPlotData(plotid_, index_, 10, td.getRightVelocity()) ;
                rb.addPlotData(plotid_, index_, 11, raccel) ;
                rb.addPlotData(plotid_, index_, 12, rout) ;                

                // XY data
                rb.addPlotData(plotid_, index_, 13, getTankDrive().getX()) ;
                rb.addPlotData(plotid_, index_, 14, getTankDrive().getY()) ;
                rb.addPlotData(plotid_, index_, 15, (lseg.getX() + rseg.getX())/ 2.0) ;
                rb.addPlotData(plotid_, index_, 16, (lseg.getY() + rseg.getY())/ 2.0) ;                                     
            }
            else {
                if (index_ == path_->size())
                    rb.endPlot(plotid_) ;
            }

            index_++ ;            
        }

        bool TankDriveFollowPathAction::isDone() {
            return index_ >= path_->size() ;
        }

        void TankDriveFollowPathAction::cancel()  {
            index_ = path_->size() ;
            std::cout << "TankDriveFollowPath canceled" << std::endl ;
        }

        std::string TankDriveFollowPathAction::toString() {
            return "TankDriveFollowPathAction-" + path_->getName() ;
        }
    }
}