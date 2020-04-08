#include "RanseurLimeLight.h"
#include "ranseurids.h"
#include <Ranseur.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::vector<std::string> cols_ =
        {
            "time", 
            "ty", "angle", "lost"
        } ;

        RanseurLimeLight::RanseurLimeLight(Subsystem *parent) : LimeLight(parent), ITerminator("Vision") {
            Robot &robot = getRobot();
            camera_angle_ = robot.getSettingsParser().getDouble("ranseurvision:camera_angle") ;
            camera_height_ = robot.getSettingsParser().getDouble("ranseurvision:camera_target_height") ;
            distance_threshold_ = robot.getSettingsParser().getDouble("ranseurvision:distance_threshold") ;
            target_count_ = robot.getSettingsParser().getInteger("ranseurvision:target_count") ;
            plotid_ = initPlot("LimeLight") ;
            prev_dist_ = std::numeric_limits<double>::max() ;
            count_ = 0 ;
            distance_ = 36 ;
        }

        RanseurLimeLight::~RanseurLimeLight() {            
        }
        
        bool RanseurLimeLight::canAcceptAction(ActionPtr ptr) {
            if (LimeLight::canAcceptAction(ptr))
                return true ;

            return false ;
        }
        void RanseurLimeLight::init(LoopType lt) {
#ifdef PLOT_LIMELIGHT
            if (lt == LoopType::Autonomous)
                startPlot(plotid_, cols_) ;
#endif
        }

        void RanseurLimeLight::reset() {
#ifdef PLOT_LIMELIGHT            
            endPlot(plotid_) ;
#endif
        }

        void RanseurLimeLight::computeMyState() {
            LimeLight::computeMyState() ;

            if (isLimeLightPresent() && isTargetPresent()) 
            {
                //
                // Valid target, use the angle method for computing distance based on the
                // limelight documentation
                //
                distance_ = camera_height_ * std::tan(xero::math::deg2rad(camera_angle_ + getTY())) ;
                yaw_ = getTX() ;
            }
            
            frc::SmartDashboard::PutNumber("Distance", distance_) ;
            frc::SmartDashboard::PutNumber("YAW", yaw_) ;

#ifdef PLOT_LIMELIGHT
            if (getRobot().IsEnabled() && getRobot().IsAutonomous())
            {
                std::vector<double> data ;
                data.push_back(getRobot().getTime()) ;
                data.push_back(getTY()) ;
                data.push_back(distance_) ;
                data.push_back(isTargetPresent()) ;
                addPlotData(plotid_, data) ;
            }
#endif
        }
        
        bool RanseurLimeLight::shouldTerminate() {
            if (!isTargetPresent()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
                logger << "RanseurLimeLight: did not detect target" ;
                logger.endMessage() ;
                count_ = 0 ;
                return false ;
            }
            
            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
            logger << "RanseurLimeLight: distance " << getDistance() << " need distance " << distance_threshold_ ;
            logger.endMessage() ;  

            double dist = getDistance() ;

            if (dist < distance_threshold_ && dist < prev_dist_)
            {
                count_++ ;
            }
            else
            {
                count_ = 0 ;
            }

            prev_dist_ = dist ;

            //
            // When we have had enough decreasing distances in a row, we trigger the
            // terminate condition
            //
            return count_ > target_count_ ;
        }
    }
}