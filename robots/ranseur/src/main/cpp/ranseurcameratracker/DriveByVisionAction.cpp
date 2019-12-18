#include "DriveByVisionAction.h"
#include "ranseurids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::vector<std::string> DriveByVisionAction::cols_ =
        {
            "time", 
            "tvel", "avel", "tdist", "adist", "yaw", "effyaw", "yawadj", "left", "right",
            "lost",
        } ;
       
        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, RanseurLimeLight &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_p_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_p") ;

            double a = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:maxa") ;
            double d = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:maxd") ;            
            double v = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:maxv") ;

            decay_factor_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:decay") ;

            camera_collector_distance_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:camera_collector_distance") ;

            profile_ = std::make_shared<TrapezoidalProfile>(a, d, v) ;
            follower_ = std::make_shared<PIDACtrl>(tank_drive.getRobot().getSettingsParser(), "drivebyvision:kv","drivebyvision:ka","drivebyvision:kp","drivebyvision:kd") ;
            plotid_ = getTankDrive().initPlot("drivebyvision") ;
        }

        void DriveByVisionAction::start() {
            lost_count_ = 0 ;
            is_done_ = false ;

            // 
            // The distance we need to travel is the reported distance from the camera to the target, 
            //
            //     minus the distance from the camera to the collector on the robot
            //            this is read as a constant from the parameter file
            //
            //     minus the distance we have traveled since the camera image was take that led to the distance report
            //            this is time * velocity
            //                velocity comes from the drivebase
            //                time is the time from the limelight pipeline latench
            //                            plus image acquisition latency (~11 ms, from params file)
            //                            plus network latency from limelight to roborio (~10 ms, from params fiel)
            //
            double dist = camera_.getDistance() - camera_collector_distance_ - camera_.getLatency() * getTankDrive().getVelocity() ;


            
            //
            // Update the trapezoidal speed profile, to match the distance to the target
            //
            profile_->update(dist, getTankDrive().getVelocity(), 0.0) ;

            //
            // Grab the start time and distance for processing the profile
            //
            profile_start_time_ = getTankDrive().getRobot().getTime() ;
            profile_start_dist_ = getTankDrive().getDist() ;

            getTankDrive().startPlot(plotid_, cols_) ;

            auto &logger = getTankDrive().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_CAMERA_TRACKER) ;
            logger << "DriveByVision:" ;
            logger << " camera " << camera_.getDistance() ;
            logger << " collector " << camera_collector_distance_ ;
            logger << " latency " << camera_.getLatency() ;
            logger << " speed " << getTankDrive().getVelocity() ;
            logger << " total " << dist ;
            logger << " dist " << dist ;
            logger << " profile " << profile_->toString() ;
            logger.endMessage() ;            
        }

        void DriveByVisionAction::run() {
            // The time from the start of the profile
            double delta = getTankDrive().getRobot().getTime() - profile_start_time_ ;

            if (delta > profile_->getTotalTime())
            {
                //
                // We are done
                //
                setMotorsToPercents(0.0, 0.0) ;
                is_done_ = true ;
                getTankDrive().endPlot(plotid_) ;
            }
            else
            {
                double yaw = 0 ;
                double lost = 1 ;
                double targeta = profile_->getAccel(delta) ;
                double targetv = profile_->getVelocity(delta) ;
                double targetdist = profile_->getDistance(delta) ;
                double actualdist = getTankDrive().getDist() - profile_start_dist_ ;
                double out = follower_->getOutput(targeta, targetv, targetdist, actualdist, 
                            getTankDrive().getRobot().getDeltaTime()) ;

                double left = out ;
                double right = out ;
                double yawadj = 0.0 ;

                yaw = camera_.getYaw() ;
                if (camera_.isTargetPresent()) {
                    lost = 0 ;
                    last_yaw_ = camera_.getYaw() ;
                }
                else
                {
                    last_yaw_ *= decay_factor_ ;
                }
                
                yawadj = camera_.getYaw() * yaw_p_ ;
                left += yawadj ;
                right -= yawadj ;   

                setMotorsToPercents(left, right) ; 

                std::vector<double> data ;
                data.push_back(delta) ;
                data.push_back(targetv) ;
                data.push_back(getTankDrive().getVelocity()) ;
                data.push_back(targetdist) ;
                data.push_back(actualdist) ;
                data.push_back(yaw) ;
                data.push_back(last_yaw_) ;
                data.push_back(yawadj);
                data.push_back(left) ;
                data.push_back(right) ;
                data.push_back(lost) ;
                getTankDrive().addPlotData(plotid_, data) ;                 
            }
        }

        /// \brief Cancel the action
        void DriveByVisionAction::cancel() { 
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;


            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision: action canceled" ;
            logger << " yaw " << camera_.getYaw() ;
            logger << " dist " << camera_.getDistance() ;
            logger.endMessage() ;             
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool DriveByVisionAction::isDone() {
            return is_done_ ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable str
        std::string DriveByVisionAction::toString() {
            std::string ret("DriveByVision") ;
            return ret ;
        }
    }
}
