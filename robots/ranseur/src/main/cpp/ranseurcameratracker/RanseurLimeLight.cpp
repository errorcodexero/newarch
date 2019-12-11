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
            "ty", "angle", "area","lost"
        } ;

        RanseurLimeLight::RanseurLimeLight(Subsystem *parent) : LimeLight(parent), ITerminator("Vision") {
            Robot &robot = getRobot();
            camera_angle_ = robot.getSettingsParser().getDouble("ranseurvision:camera_angle") ;
            camera_height_ = robot.getSettingsParser().getDouble("ranseurvision:camera_height") ;
            target_height_ = robot.getSettingsParser().getDouble("ranseurvision:target_height") ;
            distance_threshold_ = robot.getSettingsParser().getDouble("ranseurvision:distance_threshold") ;
            plotid_ = initPlot("LimeLight") ;

        }

        RanseurLimeLight::~RanseurLimeLight() {            
        }

        bool RanseurLimeLight::canAcceptAction(ActionPtr ptr) {
            if (LimeLight::canAcceptAction(ptr))
                return true ;

            return false ;
        }

        bool started = false ;
        void RanseurLimeLight::computeState() {
            LimeLight::computeState() ;

            if (getRobot().IsEnabled() && !started)
            {
                startPlot(plotid_, cols_) ;
                started = true ;
            }
            else if (getRobot().IsDisabled() && started)
            {
                endPlot(plotid_) ;
                started = false ;
            }

            if(isLimeLightPresent() && isTargetPresent()) {
                dist_angle_ = 17.75 * std::tan(xero::math::deg2rad(64.67 + getTY())) ;
            }

            dist_area_ = 44.05 / std::sqrt(getTA()) ;

            frc::SmartDashboard::PutNumber("DistArea", dist_area_) ;
            frc::SmartDashboard::PutNumber("DistAngle", dist_angle_) ;
            frc::SmartDashboard::PutNumber("TY", getTY()) ;

            if (getRobot().IsEnabled())
            {
                std::vector<double> data ;
                data.push_back(getRobot().getTime()) ;
                data.push_back(getTY()) ;
                data.push_back(dist_angle_) ;
                data.push_back(dist_area_) ;
                data.push_back(isTargetPresent()) ;
                addPlotData(plotid_, data) ;
            }

            distance_ = dist_angle_ ;
        }

        bool RanseurLimeLight::shouldTerminate() {
            if (!isTargetPresent()) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
                logger << "RanseurLimeLight: did not detect target" ;
                logger.endMessage() ;                  
                return false ;
            }
            
            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_TERMINATOR) ;
            logger << "RanseurLimeLight: distance " << getDistance() << " need distance " << distance_threshold_ ;
            logger.endMessage() ;  

            if (getDistance() > distance_threshold_)
                return false ;

            return true ;
        }
    }
}