#include "TankDriveFollowPathAction.h"
#include <tankdrive/TankDrive.h>
#include "Robot.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <cassert>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::vector<std::string> TankDriveFollowPathAction::plot_columns_ = {
            "time", 
            "ltpos", "lapos", "ltvel", "lavel", "ltaccel", "lout","lticks","lvout","laout","lpout","ldout","lerr",
            "rtpos", "rapos", "rtvel", "ravel", "rtaccel", "rout","rticks","rvout","raout","rpout","rdout","rerr",
            "thead", "ahead", "angcorr"
        } ;

        TankDriveFollowPathAction::TankDriveFollowPathAction(TankDrive &db, const std::string &name, bool reverse) : TankDriveAction(db)  {
            reverse_ = reverse;
            path_ = db.getRobot().getPathManager()->getPath(name) ;

            if (path_ == nullptr)
            {
                std::cerr << "Cannot find path '" << name << "'" << std::endl ;
                assert(path_ != nullptr) ;
            }

            std::string pname = "tankdrive:follower:";
            
            left_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), pname + "left:kv1", pname + "left:kv2",
                                pname + "left:ka", pname + "left:kp", pname + "left:kd") ;
            right_follower_ = std::make_shared<PIDACtrl>(db.getRobot().getSettingsParser(), pname + "right:kv1", pname + "left:kv2",
                                pname + "right:ka", pname + "right:kp", pname + "right:kd") ;

            turn_correction_ = db.getRobot().getSettingsParser().getDouble("tankdrive:follower:turn_correction") ;
            angle_correction_ = db.getRobot().getSettingsParser().getDouble("tankdrive:follower:angle_correction") ;

            plot_id_ = db.initPlot(toString()) ;
        }

        TankDriveFollowPathAction::~TankDriveFollowPathAction() {                
        }

        void TankDriveFollowPathAction::start() {
            left_start_ = getTankDrive().getLeftDistance() ;
            right_start_ = getTankDrive().getRightDistance() ;
            
            index_ = 0 ;         
            start_time_ = getTankDrive().getRobot().getTime() ;
            start_angle_ = getTankDrive().getAngle() ;
            target_start_angle_ = path_->getLeftSegment(0).getHeading() ;

            if (getTankDrive().hasGearShifter())
                getTankDrive().highGear() ;

            auto &logger = getTankDrive().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
            logger << "runtime" ;
            logger << ",ltpos" ;
            logger << ",lapos" ;
            logger << ",ltvel" ;
            logger << ",lout" ;
            logger << "," ;
            logger << ",rtpos" ;
            logger << ",rapos" ;
            logger << ",rtvel" ;
            logger << ",rout" ;
            logger << "," ;
            logger << ",thead" ;
            logger << ",ahead" ;
            logger << ",angerr" ;
            logger << ",turn" ;
            logger.endMessage() ;
            getTankDrive().startPlot(plot_id_, plot_columns_) ;
        }

        void TankDriveFollowPathAction::run() {
            std::vector<double> data ;
            auto &td = getTankDrive() ;
            auto &rb = td.getRobot() ;

            if (index_ < path_->size()) {
                auto &logger = td.getRobot().getMessageLogger() ;

                double dt = td.getRobot().getDeltaTime() ;
                const XeroSegment lseg = path_->getLeftSegment(index_) ;
                const XeroSegment rseg = path_->getRightSegment(index_) ;

                double laccel, lvel, lpos ;
                double raccel, rvel, rpos ;
                double thead , ahead ;

                if (reverse_) {
                    laccel = -rseg.getAccel() ;
                    lvel = -rseg.getVelocity() ;
                    lpos = -rseg.getPOS() ;
                    raccel = -lseg.getAccel() ;
                    rvel = -lseg.getVelocity() ;
                    rpos = -lseg.getPOS() ;
                    thead = xero::math::normalizeAngleDegrees(lseg.getHeading() - target_start_angle_) ;
                    ahead = xero::math::normalizeAngleDegrees(getTankDrive().getAngle() - start_angle_) ;                       
                }
                else {
                    laccel = lseg.getAccel() ;
                    lvel = lseg.getVelocity() ;
                    lpos = lseg.getPOS() ;
                    raccel = rseg.getAccel() ;
                    rvel = rseg.getVelocity() ;
                    rpos = rseg.getPOS() ;
                    thead = xero::math::normalizeAngleDegrees(lseg.getHeading() - target_start_angle_) ;  
                    ahead = xero::math::normalizeAngleDegrees(getTankDrive().getAngle() - start_angle_) ;                                      
                }

                double ldist, rdist ;

                ldist = td.getLeftDistance() - left_start_ ;
                rdist = td.getRightDistance() - right_start_ ;
                double lout = left_follower_->getOutput(laccel, lvel, lpos, ldist, dt) ;
                double rout = right_follower_->getOutput(raccel, rvel, rpos, rdist, dt) ;

#ifdef NOTYET
                //
                // This has not been tested yet.  It came looking at Mean Machine's code.  They
                // provide a boost to both sides of the robot based on the sharpness of the
                // curve the are driving.  This sharpness can be determined by the difference in
                // velocity of the left and right sides of the robot.
                //
                double dv = lseg.getVelocity() - rseg.getVelocity() ;
                double correct = dv * turn_correction_ ;
                lout += correct ;
                rout += correct ;
#endif

                double angerr = xero::math::normalizeAngleDegrees(thead - ahead) ;
                double turn = angle_correction_ * angerr ;
                lout += turn ;
                rout -= turn ;

                setMotorsToPercents(lout, rout) ;

                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                logger << td.getRobot().getTime() - start_time_ ;
                logger << "," << lpos ;
                logger << "," << ldist ;
                logger << "," << lvel ;                
                logger << "," << lout ;
                logger << "," ;
                logger << "," << rpos ;
                logger << "," << rdist ;
                logger << "," << rvel ;                
                logger << "," << rout ;
                logger << "," ;
                logger << "," << thead ;
                logger << "," << ahead ;
                logger << "," << angerr ;
                logger << "," << turn ;
                logger.endMessage() ;

                data.clear() ;
                data.push_back(rb.getTime() - start_time_) ;

                // Left side
                data.push_back(lpos) ;
                data.push_back(td.getLeftDistance() - left_start_) ;
                data.push_back(lvel) ;
                data.push_back(td.getLeftVelocity()) ;
                data.push_back(laccel) ;
                data.push_back(lout) ;
                data.push_back(td.getLeftTickCount()) ;
                data.push_back(left_follower_->getVPart()) ;
                data.push_back(left_follower_->getAPart()) ;
                data.push_back(left_follower_->getPPart()) ;
                data.push_back(left_follower_->getDPart()) ;                
                data.push_back(left_follower_->getLastError()) ;

                // Right side
                data.push_back(rpos) ;
                data.push_back(td.getRightDistance()- right_start_) ;
                data.push_back(rvel) ;
                data.push_back(td.getRightVelocity()) ;
                data.push_back(raccel) ;
                data.push_back(rout) ;
                data.push_back(td.getRightTickCount()) ;
                data.push_back(right_follower_->getVPart()) ;
                data.push_back(right_follower_->getAPart()) ;
                data.push_back(right_follower_->getPPart()) ;
                data.push_back(right_follower_->getDPart()) ; 
                data.push_back(right_follower_->getLastError()) ;

                // Angle data
                data.push_back(thead) ;
                data.push_back(ahead) ;
                data.push_back(turn) ;

                getTankDrive().addPlotData(plot_id_, data) ;
            }
            index_++ ;     
            if (index_ == path_->size())
                getTankDrive().endPlot(plot_id_) ;
        }

        bool TankDriveFollowPathAction::isDone() {
            return index_ >= path_->size() ;
        }

        void TankDriveFollowPathAction::cancel()  {
            index_ = path_->size() ;
            getTankDrive().endPlot(plot_id_) ;
        }

        std::string TankDriveFollowPathAction::toString() {
            return "TankDriveFollowPathAction-" + path_->getName() ;
        }
    }
}