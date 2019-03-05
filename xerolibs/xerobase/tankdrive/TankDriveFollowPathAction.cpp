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
            "thead", "ahead"
        } ;

        TankDriveFollowPathAction::TankDriveFollowPathAction(TankDrive &db, const std::string &name, bool reverse) : TankDriveAction(db)  {
            reverse_ = reverse;
            path_ = db.getRobot().getPathManager()->getPath(name) ;
            assert(path_ != nullptr) ;

            std::string pname = "tankdrive:follower:";
            
            left_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), pname + "left:kv", 
                                pname + "left:ka", pname + "left:kp", pname + "left:kd") ;
            right_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), pname + "right:kv", 
                                pname + "right:ka", pname + "right:kp", pname + "right:kd") ;

            turn_correction_ = db.getRobot().getSettingsParser().getDouble("tankdrive:follower:turn_correction") ;
            angle_error_ = db.getRobot().getSettingsParser().getDouble("tankdrive:follower:angle_error") ;
        }

        TankDriveFollowPathAction::~TankDriveFollowPathAction() {                
        }

        void TankDriveFollowPathAction::start() {
            left_start_ = getTankDrive().getLeftDistance() ;
            right_start_ = getTankDrive().getRightDistance() ;
            
            index_ = 0 ;         
            start_time_ = getTankDrive().getRobot().getTime() ;

            start_angle_ = getTankDrive().getAngle() ;

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
                double thead ;

                if (reverse_) {
                    laccel = -lseg.getAccel() ;
                    lvel = -lseg.getVelocity() ;
                    lpos = -lseg.getPOS() ;
                    raccel = -rseg.getAccel() ;
                    rvel = -rseg.getVelocity() ;
                    rpos = -rseg.getPOS() ;
                    thead = xero::math::normalizeAngleDegrees(-lseg.getHeading()) ;
                }
                else {
                    laccel = lseg.getAccel() ;
                    lvel = lseg.getVelocity() ;
                    lpos = lseg.getPOS() ;
                    raccel = rseg.getAccel() ;
                    rvel = rseg.getVelocity() ;
                    rpos = rseg.getPOS() ;
                    thead = xero::math::normalizeAngleDegrees(lseg.getHeading()) ;                    
                }

                double lout = left_follower_->getOutput(laccel, lvel, lpos, td.getLeftDistance() -  left_start_, dt) ;
                double rout = right_follower_->getOutput(raccel, rvel, rpos, td.getRightDistance() - right_start_, dt) ;

                double ahead = xero::math::normalizeAngleDegrees(getTankDrive().getAngle() - start_angle_) ;

                double dv = lseg.getVelocity() - rseg.getVelocity() ;
                double correct = dv * turn_correction_ ;
                lout += correct ;
                rout -= correct ;

                double angerr = lseg.getHeading() - ahead ;
                double turn = 0.8 * (-1.0 / 80.0) * angerr ;

                setMotorsToPercents(lout + turn, rout - turn) ;

                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << td.getRobot().getTime() ;
                logger << "," << td.getRobot().getTime() - start_time_ ;
                logger << "," << lpos ;
                logger << "," << td.getLeftDistance() - left_start_ ;
                logger << "," << lvel ;
                logger << "," << td.getLeftVelocity() ;
                logger << "," << laccel ;
                logger << "," << lout ;
                logger << "," << td.getLeftTickCount() ;
                logger << "," << rpos ;
                logger << "," << td.getRightDistance() - right_start_;                
                logger << "," << rvel ;
                logger << "," << td.getRightVelocity() ;                
                logger << "," << raccel ;                
                logger << "," << rout ;
                logger << "," << td.getRightTickCount() ;
                logger.endMessage() ;

                rb.addPlotData(plotid_, index_, 0, rb.getTime() - start_time_) ;

                // Left side
                rb.addPlotData(plotid_, index_, 1, lpos) ;
                rb.addPlotData(plotid_, index_, 2, td.getLeftDistance() - left_start_) ;
                rb.addPlotData(plotid_, index_, 3, lvel) ;
                rb.addPlotData(plotid_, index_, 4, td.getLeftVelocity()) ;
                rb.addPlotData(plotid_, index_, 5, laccel) ;
                rb.addPlotData(plotid_, index_, 6, lout) ;

                // Right side
                rb.addPlotData(plotid_, index_, 7, rpos) ;
                rb.addPlotData(plotid_, index_, 8, td.getRightDistance()- right_start_) ;
                rb.addPlotData(plotid_, index_, 9, rvel) ;
                rb.addPlotData(plotid_, index_, 10, td.getRightVelocity()) ;
                rb.addPlotData(plotid_, index_, 11, raccel) ;
                rb.addPlotData(plotid_, index_, 12, rout) ;                

                // XY data
                rb.addPlotData(plotid_, index_, 13, thead) ;
                rb.addPlotData(plotid_, index_, 14, ahead) ;
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