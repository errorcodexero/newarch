#include "RanseurLimeLight.h"
#include "ranseurids.h"
#include <Ranseur.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        RanseurLimeLight::RanseurLimeLight(Subsystem *parent) : 
        LimeLight(parent), ITerminator("Vision") {
            Robot &robot = getRobot();
            camera_angle_ = robot.getSettingsParser().getDouble("ranseurvision:camera_angle") ;
            camera_height_ = robot.getSettingsParser().getDouble("ranseurvision:camera_height") ;
            target_height_ = robot.getSettingsParser().getDouble("ranseurvision:target_height") ;
            distance_threshold_ = robot.getSettingsParser().getDouble("ranseurvision:distance_threshold") ;
        }

        RanseurLimeLight::~RanseurLimeLight() {            
        }

        bool RanseurLimeLight::canAcceptAction(ActionPtr ptr) {
            if (LimeLight::canAcceptAction(ptr))
                return true ;

            return false ;
        }

        void RanseurLimeLight::computeState() {
            LimeLight::computeState() ;

            double angle = xero::math::deg2rad(camera_angle_ + getTY()) ;
            if(isLimeLightPresent() && isTargetPresent()){
                dist_angle_ = (target_height_ - camera_height_)/tan(angle) ;
            }

            dist_area_ = 44.05 / std::sqrt(getTA()) ;

            frc::SmartDashboard::PutNumber("AreaVisionDist", dist_area_) ;
            frc::SmartDashboard::PutNumber("VisionDist", distance_) ;

            distance_ = dist_area_ ;
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
            logger << "RanseurLimeLight: distance " << getDistance() ;
            logger.endMessage() ;  

            if (getDistance() > distance_threshold_)
                return false ;

            return true ;
        }
    }
}